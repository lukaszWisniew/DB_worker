/*
 ============================================================================
 Name       :  DBDataProcessor.cpp
 Date       :  24-01-2024
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2024 Lukasz Wisniewski
 ============================================================================
 */

#include "DBDataProcessor.h"

#include <stddef.h>


#include "../db/DBDispatcher.h"
#include "../ev_head/Context.h"
#include "../redis_bus/frames/DataDBRequest.h"
#include "../redis_bus/frames/types/DBRequestData.h"
#include "../redis_bus/frames/DataDBResponse.h"

DBDataProcessor::DBDataProcessor() {
	_ctx = NULL;
	_dbDataMatrix.clear();

	tv_stand_by_delay.tv_sec = 0;
	tv_stand_by_delay.tv_usec = 100000;

	stand_by_ev = NULL;
}

DBDataProcessor::~DBDataProcessor() {
}

void
DBDataProcessor::setContext(Context *inCtx) {
	_ctx = inCtx;

	stand_by_ev = event_new(_ctx->_base, -1, EV_TIMEOUT, _engine_stand_by_check_cb, _ctx);

}

void
DBDataProcessor::takeData(JsonRedisMainFrame *inJsonRedisFrame) {

	DataDBRequest *frame = dynamic_cast<DataDBRequest*>(inJsonRedisFrame);

	_ctx->managerProcessor.setTrueNewFrameFromBus();

	_dbDataMatrix.clear();

	_engine(frame);

	delete inJsonRedisFrame;
	inJsonRedisFrame = NULL;
}

void
DBDataProcessor::_engine_stand_by_check_cb(evutil_socket_t fd, short what, void *ptr) {

	Context* ctx = static_cast<Context*>(ptr);

	if (!(ctx->managerProcessor.processedByOther())) {
		if (ctx->managerProcessor.emergencyAmINext()) {
		//	_setStatusAndSendQuery(inFrame);
		}
	}
}

void
DBDataProcessor::_engine(DataDBRequest *inFrame) {
	if ( !(_ctx->redisDataBus.isQueueStatus( QueueStatus::Type::START ))) {
		if ( _ctx->managerProcessor.imAlone() ) { //Jestesmy sami

			_setStatusAndSendQuery(inFrame);

		} else { //jest nas wiecej
			if ( _ctx->redisDataBus.isQueueStatus( QueueStatus::Type::OPEN )) {
				_setStatusAndSendQuery(inFrame);
			} else if ( _ctx->redisDataBus.isQueueStatus( QueueStatus::Type::STAND_BY )) {
				//TODO: Tutaj event
				event_add(stand_by_ev, &tv_stand_by_delay);
			} else if ( _ctx->redisDataBus.isQueueStatus( QueueStatus::Type::CLOSED )) {

			}  else {

			}
		}
	}
}

void
DBDataProcessor::_setStatusAndSendQuery(DataDBRequest *inFrame) {
	_ctx->managerProcessor.setQueueStatus_BLOCKED();
	_doQuery(inFrame);
}

void
DBDataProcessor::_statusAfterResponse() {


	_ctx->managerProcessor.setStatusAfterRequestDB();

/*	if ( _ctx->managerProcessor.imAlone() ) {
		_ctx->managerProcessor.setQueueStatus_OPEN();
	} else {
		//_ctx->managerProcessor.setQueueStatus_STAND_BY();
		_ctx->managerProcessor.setStatusAfterRequestDB();
	}*/
}

void
DBDataProcessor::_doQuery (DataDBRequest *inFrame) {

	if ( _ctx->dbDispatcher.query(inFrame->data.queryName, inFrame->data.variables, &_dbDataMatrix) ) {
	  _transferResponse( inFrame );
	} else {
		//TODO: Zwrocic blad
		_statusAfterResponse();
	}
}

void
DBDataProcessor::_transferResponse( DataDBRequest *inFrame ) {

	DataDBResponse* dataDBResponse = new DataDBResponse();

	dataDBResponse->setMetaDataFrame( inFrame );
	dataDBResponse->setData( _dbDataMatrix.getData() );

	_ctx->redisDataBus.sendDBDataResponse(dataDBResponse);

	_statusAfterResponse();
}

