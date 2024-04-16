/*
 ============================================================================
 Name       :  RedisDataBus.cpp
 Date       :  17-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "RedisDataBus.h"

#include <bits/types/struct_timeval.h>
#include <ev_main/EVcontext.h>
#include <event2/event.h>
#include <event2/util.h>
#include <hiredis/read.h>
#include <sched.h>
#include <cstdlib>

#include "../../ev_head/Context.h"
#include "../../processors/DBDataProcessor.h"
#include "../config/DataRBConfig.h"
#include "../config/QueueStatusType.h"
#include "../frames/DataDBRequest.h"
#include "../frames/types/QueueData.h"
#include "../manager/RedisManagerBus.h"


RedisDataBus::RedisDataBus() {
	_c_in = NULL;
	_c_out = NULL;
}

RedisDataBus::~RedisDataBus() {
	disconnect();
}

void
RedisDataBus::setContext( Context *inCtx ) {
	_RDCtx.external_ctx = inCtx;
	_RDCtx.dataRBConfig = (static_cast<Configuration*>(inCtx->getConfiguration()))->getDataRBConfig();
	_RDCtx.log = inCtx->getLog();
	_RDCtx.base = inCtx->_base;
	_RDCtx.redisDataBus = this;

	_RDCtx.redisReconnect_ev = evtimer_new (_RDCtx.base, _RedisReconnect_cb,  &_RDCtx);

	_RDCtx.pid = inCtx->pid;

	reconnect();
}

void
RedisDataBus::addToSend( const std::string inData ) {

	_sendData(inData);

}

void
RedisDataBus::disconnect() {
	redisAsyncDisconnect(_c_in);
	redisAsyncDisconnect(_c_out);
}

void
RedisDataBus::reconnect() {
	_connectToRedis();
	_attachRedisBase();
}

void
RedisDataBus::stopRedisReconnect() {

	if ( evtimer_pending( _RDCtx.redisReconnect_ev, &(_RDCtx.tv_reconnect) ) ) {
		evtimer_del(_RDCtx.redisReconnect_ev);
	}

}

void
RedisDataBus::startRedisReconnect() {
	evtimer_add (_RDCtx.redisReconnect_ev, &(_RDCtx.tv_reconnect));
}

void
RedisDataBus::startRedisToSend() {
	setQueueStatus_START();
}

void
RedisDataBus::setQueueStatus_OPEN() {
	_RDCtx.dataQueueStatus = QueueStatus::Type::OPEN;
	_RDCtx.log->sendInfo("Set Queue Status: OPEN");

	_sendQueueStatusToManager();
}
void
RedisDataBus::setQueueStatus_CLOSED() {
	_RDCtx.dataQueueStatus = QueueStatus::Type::CLOSED;
	_RDCtx.log->sendInfo("Set Queue Status: CLOSED");

	_sendQueueStatusToManager();
}
void
RedisDataBus::setQueueStatus_START() {
	_RDCtx.dataQueueStatus = QueueStatus::Type::START;
	_RDCtx.log->sendInfo("Set Queue Status: START");

	_sendQueueStatusToManager();
}
void
RedisDataBus::setQueueStatus_BLOCKED() {
	_RDCtx.dataQueueStatus = QueueStatus::Type::BLOCKED;
	_RDCtx.log->sendInfo("Set Queue Status: BLOCKED");

	_sendQueueStatusToManager();
}

void
RedisDataBus::setQueueStatus_STAND_BY() {
	_RDCtx.dataQueueStatus = QueueStatus::Type::STAND_BY;
	_RDCtx.log->sendInfo("Set Queue Status: STAND_BY");

	_sendQueueStatusToManager();
}


void
RedisDataBus::sendDBDataResponse( DataDBResponse *inDataDBResponse ) {

	addToSend(inDataDBResponse->toJson());
	delete inDataDBResponse;

}

bool
RedisDataBus::isQueueStatus( QueueStatus::Type inDataQueueStatus ) {
	bool outStatus = false;

	if ( _RDCtx.dataQueueStatus == inDataQueueStatus ) {
		outStatus = true;
	}

	return outStatus;
}

QueueStatus::Type
RedisDataBus::getQueueStatus() {
	return _RDCtx.dataQueueStatus;
}

void
RedisDataBus::_sendData(const std::string inData) {

	if ( _RDCtx.redisDataConnected ) {
		_publish(inData);
	}
}

bool
RedisDataBus::_publish(const std::string inData) {
	bool outStatus = false;
	if ( _RDCtx.redisDataConnected ) {
		std::string outCommand = "PUBLISH " + _RDCtx.dataRBConfig->chName + " %s";
		std::string debugMess = outCommand + "; DATA: " + inData;
		_RDCtx.log->sendDebug(debugMess);
		redisAsyncCommand(_c_out, NULL, NULL, outCommand.c_str(), inData.c_str(), inData.size());
		if (_c_out->err) {
			std::string error = "Publish DataBUS: " + std::string(_c_in->errstr);
			_RDCtx.log->sendError(error);
		} else {
			std::string infoMess = "Message published on " + _RDCtx.dataRBConfig->chName + " channel.";
			_RDCtx.log->sendInfo(infoMess);
			outStatus = true;
		}
	}
	return outStatus;
}

void
RedisDataBus::_connectToRedis() {

	std::string ip = _RDCtx.dataRBConfig->ip;
	int port = _RDCtx.dataRBConfig->port;

	_connectInRedis(ip, port);
	_connectOutRedis(ip, port);
}

void
RedisDataBus::_connectInRedis(const std::string &inIP, const int inPort) {

	_c_in = redisAsyncConnect(inIP.c_str(), inPort);

	if (_c_in->err) {
		std::string errorMess = "Watcher DataBUS: " + std::string(_c_in->errstr);
		_RDCtx.log->sendError(errorMess);
		redisAsyncFree(_c_in);
		_c_in = NULL;
		exit(EXIT_FAILURE);
	} else {
		_c_in->data = &_RDCtx;
		redisAsyncSetConnectCallback(_c_in, _appOnConnect);
		redisAsyncSetDisconnectCallback(_c_in, _appOnDisconnect);
		_RDCtx.redisDataConnected = true;
	}
}
void
RedisDataBus::_connectOutRedis(const std::string &inIP, const int inPort) {

	_c_out = redisAsyncConnect(inIP.c_str(), inPort);
	if (_c_out->err) {
		std::string errorMess = "Sender DataBUS: " + std::string(_c_out->errstr);
		_RDCtx.log->sendError(errorMess);

		redisAsyncFree(_c_out);
		_c_out = NULL;
		exit(EXIT_FAILURE);
	} else {
		_c_out->data = &_RDCtx;
		redisAsyncSetConnectCallback(_c_out, _appOnConnect);
		redisAsyncSetDisconnectCallback(_c_out, _appOnDisconnect);
		_RDCtx.redisDataConnected = true;
	}

}

void
RedisDataBus::_appOnConnect(const redisAsyncContext *c, int status) {

	RedisDataCtx* _ctx = static_cast<RedisDataCtx*>(c->data);
	if (status == REDIS_OK) {

		_ctx->redisDataBus->stopRedisReconnect();
		_ctx->redisDataConnected = true;
		std::string debugMess = "DataBUS: Connection to Redis Manager BUS established";
		//std::cerr << debugMess << std::endl;
		_ctx->log->sendDebug(debugMess);
	} else {
		_ctx->redisDataConnected = false;

		std::string errorMess = "DataBUS: I can't establish connection to Redis Manager BUS.";
		_ctx->log->sendError(errorMess);

		_ctx->redisDataBus->startRedisReconnect();
	}
}

void
RedisDataBus::_appOnDisconnect(const redisAsyncContext *c, int status) {

	RedisDataCtx* _ctx = static_cast<RedisDataCtx*>(c->data);
	_ctx->redisDataConnected = false;
	if (status == REDIS_OK) {
		_ctx->redisDataBus->stopRedisReconnect();

		std::string debugMess = "DataBUS: Disconnected Redis Manager BUS";
		_ctx->log->sendDebug(debugMess);
	} else {
		std::string errorMess = "DataBUS: Connection to Redis Manager BUS is broken.";
		_ctx->log->sendError(errorMess);
		_ctx->redisDataBus->startRedisReconnect();
	}
}

void
RedisDataBus::_attachRedisBase() {

	redisLibeventAttach(_c_in, _RDCtx.base);
	redisLibeventAttach(_c_out, _RDCtx.base);

	_authToRedis();
	_subscribe();
}

void
RedisDataBus::_authToRedis() {

	std::string auth = "AUTH " + _RDCtx.dataRBConfig->password;

	redisAsyncCommand(_c_in, NULL, NULL, auth.c_str());
	if (_c_in->err) {
		std::string errorMess = "DataBUS[IN]: User not authorized: " + std::string(_c_in->errstr);
		_RDCtx.log->sendError(errorMess);
		exit(EXIT_FAILURE);
	} else {
		std::string debugMess = "DataBUS[IN]: User authorized." ;
		_RDCtx.log->sendDebug(debugMess);
	}

	redisAsyncCommand(_c_out, NULL, NULL, auth.c_str());
	if (_c_out->err) {
		std::string errorMess = "DataBUS[OUT]: User not authorized: " + std::string(_c_in->errstr);
		_RDCtx.log->sendError(errorMess);
		exit(EXIT_FAILURE);
	} else {
		std::string debugMess = "DataBUS[OUT]: User authorized." ;
		_RDCtx.log->sendDebug(debugMess);
	}
}

void
RedisDataBus::_subscribe() {
	std::string subscribe = "SUBSCRIBE " + _RDCtx.dataRBConfig->chName;

	redisAsyncCommand(_c_in, _onMessage, &_RDCtx, subscribe.c_str());
	if (_c_in->err) {
		std::string errorMess = "DataBUS: Not subscribed to the channel. " + std::string(_c_in->errstr);
		_RDCtx.log->sendError(errorMess);
		exit(EXIT_FAILURE);
	} else {
		std::string debugMess = "DataBUS: channel " + _RDCtx.dataRBConfig->chName + "is subscribed.";
		_RDCtx.log->sendDebug(debugMess);
	}
}

void
RedisDataBus::_sendQueueStatusToManager() {

	DataQueueStatus dataQueueStatus;

	dataQueueStatus.pid = _RDCtx.pid;


	dataQueueStatus.data.dataQueueStatus = _RDCtx.dataQueueStatus;

	( (Context *) (_RDCtx.external_ctx))->redisManagerBus.addToSend(dataQueueStatus.toJson());

}

void
RedisDataBus::_RedisReconnect_cb( evutil_socket_t fd, short what, void *ptr ) {

	RedisDataCtx *ctx = static_cast<RedisDataCtx*>(ptr);

	std::string debugMess = "Context: Redis Data BUS reconnect.";
	ctx->log->sendDebug(debugMess);

	ctx->redisDataBus->reconnect();

	evtimer_add (ctx->redisReconnect_ev, &(ctx->tv_reconnect));

}

void
RedisDataBus::_onMessage(redisAsyncContext *c, void *reply, void *privdata) {
	redisReply *r = static_cast<redisReply*>(reply);
	RedisDataCtx* ctx = static_cast<RedisDataCtx*>(privdata);

	std::string errorJson;

	if (reply == NULL) return;

	if (r->type == REDIS_REPLY_ARRAY) {

		if (( 3 == r->elements ) && ( !strcmp((r->element[0]->str), "message") ) && ( !strcmp((r->element[1]->str), ctx->dataRBConfig->chName.c_str() ) )) {
			if (0 < r->element[2]->len) {
				if (DataDBRequest::itIsJson(r->element[2]->str) && DataDBRequest::itIsMyDomain(r->element[2]->str, ctx->dataRBConfig->getDomain())) {

					DataDBRequest* dataDBRequestFrame = new DataDBRequest();

					if ( dataDBRequestFrame->fromJson( r->element[2]->str, errorJson ) ) {

						ctx->log->sendDebug( "RedisDataBus::onMessage: " + dataDBRequestFrame->dataToJson() );
						(static_cast<Context*>((ctx->external_ctx)))->dbDataProcessor.takeData(dataDBRequestFrame);

					} else {
						if (!errorJson.empty()) ctx->log->sendError( errorJson );
						delete dataDBRequestFrame;
					}

				} else {
					ctx->log->sendInfo( "RedisDataBus::onMessage: Unknown frame type. ( " + std::string(r->element[2]->str) + ")");
				}
			}
		}
	}
}
