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
}

DBDataProcessor::~DBDataProcessor() {
}

void
DBDataProcessor::setContext(Context *inCtx) {
	_ctx = inCtx;
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
DBDataProcessor::_engine(DataDBRequest *inFrame) {
	if ( !(_ctx->redisDataBus.isQueueStatus( QueueStatus::Type::START ))) {
		if ( _ctx->managerProcessor.imAlone() ) { //Jestesmy sami

			_setStatusAndSendQuery(inFrame);

		} else { //jest nas wiecej
			if ( _ctx->redisDataBus.isQueueStatus( QueueStatus::Type::OPEN )) {
				_setStatusAndSendQuery(inFrame);
			} else if ( _ctx->redisDataBus.isQueueStatus( QueueStatus::Type::STAND_BY )) {
				if (!(_ctx->managerProcessor.processedByOther())) {
					if (_ctx->managerProcessor.emergencyAmINext()) {
						_setStatusAndSendQuery(inFrame);
					}
				}
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

	if ( _ctx->managerProcessor.imAlone() ) {
		_ctx->managerProcessor.setQueueStatus_OPEN();
	} else {
		_ctx->managerProcessor.setQueueStatus_STAND_BY();
	}
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

