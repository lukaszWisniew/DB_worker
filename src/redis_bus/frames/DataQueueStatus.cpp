/*
 ============================================================================
 Name       :  DataQueueStatus.cpp
 Date       :  30-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "DataQueueStatus.h"

#include "../config/QueueStatusType.h"

#include <exception>

std::string DataQueueStatus::_frameName = "dbWorkerStatus";

DataQueueStatus::DataQueueStatus() {
	frameType = _frameName;

	clear();
}

DataQueueStatus::~DataQueueStatus() {}


std::string
DataQueueStatus::toJson() {
	j = (*this);
	return j.dump();
}

bool
DataQueueStatus::fromJson( const std::string& inJSON, std::string& outError  ) {

	std::string jsonFT; //Typ ramki odczytany z JSONa

	_strData.clear();

	try {

		j = json::parse(inJSON);

		jsonFT = j.at("frameType").get<std::string>();

		//{"data":{"dataQueueStatus":"start","maxSize":10,"openSize":5,"size":0},"frameType":"dataQueueStatus","pid":91612,"version":"1.0"}

		if ( !frameType.compare(jsonFT) ) {
			try {
				pid = j.at("/pid"_json_pointer).get<int>();
				data = j.at("data").get<QueueData>();

			} catch (const std::exception& e){
				outError = "DataQueueStatus::fromJson::data: " + std::string(e.what());
				clear();
				return false;
			}
			_strData = inJSON;
			return true;
		} else {

		}


	} catch(const std::exception& e) {
		//inRMCtx.log->sendDebug("JSON parse error in \"DataQueueStatus::fromJson\"");
		outError = "DataQueueStatus::fromJson::jsonParse: " + std::string(e.what());
		clear();
	}


	return false;
}

std::string
DataQueueStatus::getQueueStatusS() {
	return QueueStatus::typeToString(data.dataQueueStatus);
}

bool
DataQueueStatus::itIsJson(const std::string& inJSON) {
	bool outStatus = false;
	std::string jsonFT; //Typ ramki odczytany z JSONa
	json j = json::parse(inJSON);

	jsonFT = j.at("frameType").get<std::string>();

	if ( !jsonFT.compare(_frameName) ) {
		outStatus = true;
	}

	return outStatus;
}

bool
DataQueueStatus::itIsString(const std::string& inFrameType) {
	bool outStatus = false;

	if ( !inFrameType.compare(_frameName)  ) outStatus = true;

	return outStatus;
}

QueueData&
DataQueueStatus::getData() {
	return data;
}


void
DataQueueStatus::clear() {
	pid             = -1;
	data.clear();
}

void
DataQueueStatus::showData() {
	std::cout << "QUEUE STATUS: " <<  QueueStatus::typeToString(data.dataQueueStatus) << "\n"
/*			  << "MAX SIZE: " << data.maxSize << "\n"
			  << "OPEN SIZE: " << data.openSize << "\n"
			  << "SIZE: " << data.size << "\n"*/
			  << "PID: " << pid << std::endl;
}

