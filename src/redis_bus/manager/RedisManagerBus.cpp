/*
 ============================================================================
 Name       :  RedisManagerBus.cpp
 Date       :  02-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "RedisManagerBus.h"

#include <bits/types/struct_timeval.h>
#include <ev_main/EVcontext.h>
#include <event2/event.h>
#include <event2/util.h>
#include <hiredis/read.h>
#include <stddef.h>
#include <cstdlib>
#include <iostream>

#include "../../ev_head/Context.h"
#include "../config/ManagerRBConfig.h"
#include "../frames/DataQueueStatus.h"
#include "../frames/DBWorkersStatus.h"


RedisManagerBus::RedisManagerBus() {

	_c_in = NULL;
	_c_out = NULL;

}

RedisManagerBus::~RedisManagerBus() {
	disconnect();
}

void
RedisManagerBus::setContext( Context *inCtx ) {
	_RMCtx.external_ctx = inCtx;
	_RMCtx.managerRBConfig = (static_cast<Configuration*>(inCtx->getConfiguration()))->getManagerRBConfig();
	_RMCtx.log = inCtx->getLog();
	_RMCtx.base = inCtx->_base;
	_RMCtx.redisManagerBus = this;

	_RMCtx.redisReconnect_ev = evtimer_new (_RMCtx.base, _RedisReconnect_cb,  &_RMCtx);

	reconnect();

}

void
RedisManagerBus::addToSend( const std::string inData ) {

	_sendData (inData);

}

void
RedisManagerBus::disconnect() {
	redisAsyncDisconnect(_c_in);
	redisAsyncDisconnect(_c_out);
}

void
RedisManagerBus::reconnect() {
	_connectToRedis();
	_attachRedisBase();
}

void
RedisManagerBus::stopRedisReconnect() {

	if ( evtimer_pending( _RMCtx.redisReconnect_ev, &(_RMCtx.tv_reconnect) ) ) {
		evtimer_del(_RMCtx.redisReconnect_ev);
	} else {

	}
}

void
RedisManagerBus::startRedisReconnect() {

	if (!evtimer_pending(_RMCtx.redisReconnect_ev, &(_RMCtx.tv_reconnect))) {

		evtimer_add (_RMCtx.redisReconnect_ev, &(_RMCtx.tv_reconnect));
	} else {

	}
}

void
RedisManagerBus::_sendData(const std::string inData) {
	if (_RMCtx.redisMgrConnected) {
		_publish(inData);
	}
}

bool
RedisManagerBus::_publish(const std::string inData) {
	bool outStatus = false;

	if ( _RMCtx.redisMgrConnected ) {

		std::string outCommand = "PUBLISH " + _RMCtx.managerRBConfig->chName + " %s";
		std::string debugMess = outCommand + "; DATA: " + inData;
		_RMCtx.log->sendDebug(debugMess);

		redisAsyncCommand(_c_out, NULL, NULL, outCommand.c_str(), inData.c_str());
		if (_c_out->err) {
			std::string error = "Publish ManagerBUS: " + std::string(_c_in->errstr);
			_RMCtx.log->sendError(error);
		} else {
			std::string infoMess = "Message published on " + _RMCtx.managerRBConfig->chName + " channel.";
			_RMCtx.log->sendInfo(infoMess);
			outStatus = true;
		}
	}
	return outStatus;
}

void
RedisManagerBus::_connectToRedis() {

	std::string ip = _RMCtx.managerRBConfig->ip;
	int port = _RMCtx.managerRBConfig->port;

	_connectInRedis(ip, port);
	_connectOutRedis(ip, port);
}

void
RedisManagerBus::_connectInRedis(const std::string &inIP, const int inPort) {

	_c_in = redisAsyncConnect(inIP.c_str(), inPort);

	if (_c_in->err) {
		std::string errorMess = "Watcher ManagerBUS: " + std::string(_c_in->errstr);
		_RMCtx.log->sendError(errorMess);

		redisAsyncFree(_c_in);
		_c_in = NULL;
		exit(EXIT_FAILURE);
	} else {
		_c_in->data = &_RMCtx;
		redisAsyncSetConnectCallback(_c_in, _appOnConnect);
		redisAsyncSetDisconnectCallback(_c_in, _appOnDisconnect);
		_RMCtx.redisMgrConnected = true;
	}
}
void
RedisManagerBus::_connectOutRedis(const std::string &inIP, const int inPort) {

	_c_out = redisAsyncConnect(inIP.c_str(), inPort);
	if (_c_out->err) {
		std::string errorMess = "Sender ManagerBUS: " + std::string(_c_out->errstr);
		_RMCtx.log->sendError(errorMess);

		redisAsyncFree(_c_out);
		_c_out = NULL;
		exit(EXIT_FAILURE);
	} else {
		_c_out->data = &_RMCtx;
		redisAsyncSetConnectCallback(_c_out, _appOnConnect);
		redisAsyncSetDisconnectCallback(_c_out, _appOnDisconnect);
		_RMCtx.redisMgrConnected = true;
	}

}

void
RedisManagerBus::_appOnConnect(const redisAsyncContext *c, int status) {

	RedisManagerCtx *ctx = static_cast<RedisManagerCtx*>(c->data);
	if (status == REDIS_OK) {

		ctx->redisManagerBus->stopRedisReconnect();
		ctx->redisMgrConnected = true;

		std::string debugMess = "ManagerBUS: Connection to Redis Manager BUS established";
		ctx->log->sendDebug(debugMess);
	} else {
		ctx->redisMgrConnected = false;

		std::string errorMess = "ManagerBUS: I can't establish connection to Redis Manager BUS.";
		ctx->log->sendError(errorMess);

		ctx->redisManagerBus->startRedisReconnect();
	}

}
void
RedisManagerBus::_appOnDisconnect(const redisAsyncContext *c, int status) {

	RedisManagerCtx *ctx = static_cast<RedisManagerCtx*>(c->data);
	ctx->redisMgrConnected = false;
	if (status == REDIS_OK) {
		ctx->redisManagerBus->stopRedisReconnect();

		std::string debugMess = "ManagerBUS: Disconnected Redis Manager BUS";
		ctx->log->sendDebug(debugMess);
	} else {
		std::string errorMess = "ManagerBUS: Connection to Redis Manager BUS is broken.";
		ctx->log->sendError(errorMess);

		ctx->redisManagerBus->startRedisReconnect();
	}
}

void
RedisManagerBus::_attachRedisBase() {

	redisLibeventAttach(_c_in, _RMCtx.base);
	redisLibeventAttach(_c_out, _RMCtx.base);

	_authToRedis();
	_subscribe();
}

void
RedisManagerBus::_authToRedis() {

	std::string auth = "AUTH " + _RMCtx.managerRBConfig->password;

	redisAsyncCommand(_c_in, NULL, NULL, auth.c_str());
	if (_c_in->err) {
		std::string errorMess = "ManagerBUS[IN]: User not authorized: " + std::string(_c_in->errstr);
		_RMCtx.log->sendError(errorMess);
		exit(EXIT_FAILURE);
	} else {
		std::string debugMess = "ManagerBUS[IN]: User authorized." ;
		_RMCtx.log->sendDebug(debugMess);
	}

	redisAsyncCommand(_c_out, NULL, NULL, auth.c_str());
	if (_c_out->err) {
		std::string errorMess = "ManagerBUS[OUT]: User not authorized: " + std::string(_c_in->errstr);
		_RMCtx.log->sendError(errorMess);
		exit(EXIT_FAILURE);
	} else {
		std::string debugMess = "ManagerBUS[OUT]: User authorized." ;
		_RMCtx.log->sendDebug(debugMess);
	}
}

void
RedisManagerBus::_subscribe() {
	std::string subscribe = "SUBSCRIBE " + _RMCtx.managerRBConfig->chName;

	redisAsyncCommand(_c_in, _onMessage, &_RMCtx, subscribe.c_str());
	if (_c_in->err) {
		std::string errorMess = "ManagerBUS: Not subscribed to the channel. " + std::string(_c_in->errstr);
		_RMCtx.log->sendError(errorMess);
		exit(EXIT_FAILURE);
	} else {
		std::string debugMess = "ManagerBUS: channel " + _RMCtx.managerRBConfig->chName + " is subscribed.";
		_RMCtx.log->sendDebug(debugMess);
	}
}

void
RedisManagerBus::_RedisReconnect_cb( evutil_socket_t fd, short what, void *ptr ) {

	RedisManagerCtx *ctx = static_cast<RedisManagerCtx*>(ptr);

	std::string debugMess = "Context: Redis Manager BUS reconnect.";
	ctx->log->sendDebug(debugMess);

	ctx->redisManagerBus->reconnect();

}

void
RedisManagerBus::_onMessage(redisAsyncContext *c, void *reply, void *privdata) {
	redisReply *r = static_cast<redisReply*>(reply);
	RedisManagerCtx* ctx = static_cast<RedisManagerCtx*>(privdata);

	std::string errorJson;

	if (reply == NULL) return;

	if (r->type == REDIS_REPLY_ARRAY) {

		if (( 3 == r->elements ) && ( !strcmp((r->element[0]->str), "message") ) && ( !strcmp((r->element[1]->str), ctx->managerRBConfig->chName.c_str() ) )) {
			if (0 < r->element[2]->len) {

				if (DataQueueStatus::itIsJson(r->element[2]->str)) {

					DataQueueStatus* busDataQueueStatusFrame = new DataQueueStatus();

					if ( busDataQueueStatusFrame->fromJson( r->element[2]->str, errorJson ) ) {
						if (busDataQueueStatusFrame->pid == (static_cast<Context*>((ctx->external_ctx)))->pid) {
							ctx->log->sendDebug( "RedisManagerBus::onMessage: Back message." );
							delete busDataQueueStatusFrame;
						} else {
							ctx->log->sendDebug( "RedisManagerBus::onMessage: " + busDataQueueStatusFrame->toJson() );
							(static_cast<Context*>((ctx->external_ctx)))->managerProcessor.takeData(busDataQueueStatusFrame);
						}

					} else {
						if (!errorJson.empty()) ctx->log->sendDebug( errorJson );
						delete busDataQueueStatusFrame;
					}
				} else if (DBWorkersStatus::itIsJson(r->element[2]->str)) {

					DBWorkersStatus* busDBWorkersStatusFrame = new DBWorkersStatus();

					if ( busDBWorkersStatusFrame->fromJson( r->element[2]->str, errorJson ) ) {
						ctx->log->sendDebug( "RedisManagerBus::onMessage: " + busDBWorkersStatusFrame->dataToJson() );
						(static_cast<Context*>((ctx->external_ctx)))->managerProcessor.takeData(busDBWorkersStatusFrame);
					} else {
						if (!errorJson.empty()) ctx->log->sendDebug( errorJson );
						delete busDBWorkersStatusFrame;
					}

				} else {
					ctx->log->sendInfo( "RedisManagerBus::onMessage: Unknown frame type. ( " + std::string(r->element[2]->str) + ")");
				}
			} else {
				//ERROR
			}
		}
	}
}
