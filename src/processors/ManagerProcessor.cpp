/*
 ============================================================================
 Name       :  ManagerProcessor.cpp
 Date       :  12-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "ManagerProcessor.h"

#include "../redis_bus/frames/DataQueueStatus.h"
#include "../redis_bus/frames/DBWorkersStatus.h"

#include "../ev_head/Context.h"

ManagerProcessor::ManagerProcessor() {
	_ctx = NULL;
	_setNotSetNewFrameFromBus();
	_clearOldStatus();
}

ManagerProcessor::~ManagerProcessor() {}

void
ManagerProcessor::setContext( Context *inCtx ) {
	_ctx = inCtx;
	_pidStatus[_ctx->pid] = _ctx->redisDataBus.getQueueStatus();
}

void
ManagerProcessor::takeData( JsonRedisMainFrame* inJsonRedisFrame ) {

	if ( DataQueueStatus::itIsString(inJsonRedisFrame->frameType)) {
		_setStatus((dynamic_cast<DataQueueStatus*>(inJsonRedisFrame))->getData(), (dynamic_cast<DataQueueStatus*>(inJsonRedisFrame))->pid);
	} else if (DBWorkersStatus::itIsString(inJsonRedisFrame->frameType)) {
		_setStatuses((dynamic_cast<DBWorkersStatus*>(inJsonRedisFrame))->getData());
	}

	delete inJsonRedisFrame;
	inJsonRedisFrame = NULL;
}

bool
ManagerProcessor::imAlone() {
	bool outStatus = false;

	if ( 1 == _pidStatus.size() ) outStatus = true;

	return outStatus;
}

void
ManagerProcessor::setQueueStatus_OPEN() {
	_setQueueStatus_OPEN();
}
void
ManagerProcessor::setQueueStatus_CLOSED() {
	_setQueueStatus_CLOSED();
}
void
ManagerProcessor::setQueueStatus_BLOCKED() {
	_setQueueStatus_BLOCKED();
}

void
ManagerProcessor::setQueueStatus_STAND_BY() {
	_setQueueStatus_STAND_BY();
}

bool
ManagerProcessor::processedByOther() {
	bool outStatus = false;

	if ( 0 == _newFrameFromBus ) {
		outStatus = true;
	}

	return outStatus;
}

void
ManagerProcessor::setTrueNewFrameFromBus() {
	_setTrueNewFrameFromBus();
}
void
ManagerProcessor::setProcessedNewFrameFromBus() {
	_setProcessedNewFrameFromBus();
}
void
ManagerProcessor::setNotSetNewFrameFromBus() {
	_setNotSetNewFrameFromBus();
}

bool
ManagerProcessor::emergencyAmINext() {
	bool outStatus = false;
	int pid = 0;

	if ( _getOpenPID(pid) ){
		if ( _amInext(pid) ) {
			outStatus = true;
		}
	}

	return outStatus;
}

void
ManagerProcessor::_setStatuses( WorkersStatusData& inWorkersStatusData ) {
	for ( auto d : inWorkersStatusData.wqs ) {
		_pidStatus[d.pid] = QueueStatus::typeFromString(d.status);
	}
	_engine();
}

void
ManagerProcessor::_setStatus(  QueueData& inDataQueueStatusData, int inPid ) {
	auto it = _pidStatus.find(inPid);

	if (it != _pidStatus.end()){
		_old_status = std::make_pair(it->first, it->second);
	};
	_pidStatus[inPid] = inDataQueueStatusData.dataQueueStatus;

	_engine();
}

void
ManagerProcessor::_engine() {

	int pid = 0;

	if ( _ctx != NULL ) {

		if ( imAlone() ) { //Jezeli jest tylko jedna instancja DB_workera pomijamy limity kolejki
			_setQueueStatus_OPEN();
		} else {
			/*
			 * PARA [<PID>, <Status kolejki>] (_old_status)
			 * Jesli para == [0,none]
			 * 	Pobieramy jaki pid jest OPEN
			 * 	Jesli brak -> MY OPEN
			 * 	w przeciwnym przypadku -> MY CLOSED
			 * Jesli para != [0,none] -> para ma stary status/jest ustawiony
			 * Jesli stary status byl OPENED a teraz jest CLOSED lub BLOCKED
			 *  Sprawdzamy czy kolejny PID nie nasz
			 *  	Jesli NIE nic nie rob
			 *  	Jesli TAK
			 *  	MY OPEN
			 *  	---- deprecated ----
			 *  		Jesli kolejka status BLOCKED
			 *  			TAK:
			 *  		  		Sprawdzic czy ponizej opened
			 *  		  			TAK -> MY OPEN
			 *  		  			NIE -> nic nie rob
			 *  		  	NIE:
			 *  		  		Sprawdzic czy osiagnieto max
			 *  		  			TAK -> MY BLOCKED
			 *  		  			NIE -> MY OPENED
			 */

			if ( _old_status.first == 0 ) {
				if (_getOpenPID(pid)) { //Pobieramy jaki pid jest OPEN
					//Jest ktos otwarty ->MY STAND_BY
					if (!(_ctx->redisDataBus.isQueueStatus( QueueStatus::Type::BLOCKED))) {
						_setQueueStatus_STAND_BY();
					}

				} else if ( _getClosedPID(pid) ) {
					if (_amInext(pid)) {
						_setQueueStatus_OPEN();
					} else {
						_setQueueStatus_STAND_BY();
					}

				} else { //Jesli brak -> MY OPEN
					_setQueueStatus_OPEN();
				}
			} else {
				//Jesli stary status byl OPENED a teraz jest CLOSED lub BLOCKED
				if ( (QueueStatus::Type::OPEN == _old_status.second) &&
					 ((QueueStatus::Type::CLOSED == _pidStatus[_old_status.first]) || (QueueStatus::Type::BLOCKED == _pidStatus[_old_status.first]) || (QueueStatus::Type::STAND_BY == _pidStatus[_old_status.first]) )
				   ) {
					//Sprawdzamy czy kolejny PID to my
					if (_amInext(_old_status.first)) {
						_setQueueStatus_OPEN();
					} else if ( (QueueStatus::Type::OPEN == _old_status.second) && (QueueStatus::Type::CLOSED == _pidStatus[_old_status.first]) ) {
						//Jakis proces przetwarza nowe zapytanie
						_setProcessedNewFrameFromBus();
				    } else if ( (QueueStatus::Type::CLOSED == _old_status.second) && (QueueStatus::Type::STAND_BY == _pidStatus[_old_status.first]) ) {
						//Jakis proces przetworzyl i czeka na zadanie
				    	_setNotSetNewFrameFromBus();
				    } else {
						//NIE -> nic nie rob
					}
				} else {

				}
			}
			_clearOldStatus();
		}
	}
}

bool
ManagerProcessor::_amInext( int &inPid ) {
	bool outStatus = false;

	int nextPID = -1;

	nextPID = _findNext(inPid);

	if ( _ctx != NULL ) {
		if ( -1 < nextPID ) {
			if ( nextPID == _ctx->pid ) {
				outStatus = true;
			} else {

				for (; (nextPID != _ctx->pid) && (QueueStatus::Type::STAND_BY !=  _pidStatus[nextPID]) ; nextPID = _findNext(nextPID) );
				if ( nextPID == _ctx->pid ) {
					outStatus = true;
				}

			}
		}
	}

	return outStatus;
}

int
ManagerProcessor::_findNext( int &inPid ) {
	int outPid = -1;

	auto it  = _pidStatus.find(inPid);

	if ( it != _pidStatus.end() ) it++;
	if ( it == _pidStatus.end() ) {
		outPid = _pidStatus.begin()->first;
	} else {
		outPid = it->first;
	}

	return outPid;
}

bool
ManagerProcessor::_getOpenPID(int &outPid) {
	bool outStatus = false;
	auto it = _pidStatus.begin();


	for (; ((it != _pidStatus.end()) && (it->second != QueueStatus::Type::OPEN)); ++it);

	if (it != _pidStatus.end()) {
		outPid = it->first;
		outStatus = true;
	} else {
		outPid = 0;
	}

	return outStatus;
}

bool
ManagerProcessor::_getClosedPID(int &outPid) {
	bool outStatus = false;
	auto it = _pidStatus.begin();

	outPid = 0;

	for (; ((it != _pidStatus.end()) && (it->second != QueueStatus::Type::CLOSED)); ++it);

	if (it != _pidStatus.end()) {
		outPid = it->first;
		outStatus = true;
	} else {
		outPid = 0;
	}


	return outStatus;
}

void
ManagerProcessor::_clearOldStatus() {
	_old_status = std::make_pair(0,QueueStatus::Type::none);
}

void
ManagerProcessor::_setQueueStatus_OPEN() {
	if (_ctx != NULL ) {
		if (!(_ctx->redisDataBus.isQueueStatus( QueueStatus::Type::OPEN))) { //Czy nie jesteśmy juz OPEN?
			_ctx->redisDataBus.setQueueStatus_OPEN();
			_pidStatus[_ctx->pid] = _ctx->redisDataBus.getQueueStatus();
		}
	}
}

void
ManagerProcessor::_setQueueStatus_CLOSED() {
	if (_ctx != NULL ) {
		if (!(_ctx->redisDataBus.isQueueStatus( QueueStatus::Type::CLOSED))) { //Czy nie jesteśmy juz CLOSED?
			_ctx->redisDataBus.setQueueStatus_CLOSED();
			_pidStatus[_ctx->pid] = _ctx->redisDataBus.getQueueStatus();
		}
	}
}

void
ManagerProcessor::_setQueueStatus_BLOCKED() {
	if (_ctx != NULL ) {
		_ctx->redisDataBus.setQueueStatus_BLOCKED();
		_pidStatus[_ctx->pid] = _ctx->redisDataBus.getQueueStatus();
	}
}

void
ManagerProcessor::_setQueueStatus_STAND_BY() {
	if (_ctx != NULL ) {
		if (!(_ctx->redisDataBus.isQueueStatus( QueueStatus::Type::STAND_BY))) { //Czy nie jesteśmy juz STAND_BY?
			_ctx->redisDataBus.setQueueStatus_STAND_BY();
			_pidStatus[_ctx->pid] = _ctx->redisDataBus.getQueueStatus();
		}
	}
}

void
ManagerProcessor::_setTrueNewFrameFromBus() {
	_newFrameFromBus = 1;
}
void
ManagerProcessor::_setProcessedNewFrameFromBus() {
	_newFrameFromBus = 0;
}
void
ManagerProcessor::_setNotSetNewFrameFromBus() {
	_newFrameFromBus = -1;
}

