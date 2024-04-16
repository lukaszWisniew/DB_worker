/*
 ============================================================================
 Name       :  DBWorkersStatus.cpp
 Date       :  12-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "DBWorkersStatus.h"

#include <iostream>

std::string DBWorkersStatus::_frameName = "dbWorkersStatus";

DBWorkersStatus::DBWorkersStatus() {
	frameType = _frameName;
	clear();
}

DBWorkersStatus::~DBWorkersStatus() {}

void
DBWorkersStatus::clear() {
	_data.clear();
}

bool
DBWorkersStatus::itIsJson(const std::string& inJSON) {
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
DBWorkersStatus::itIsString(const std::string& inFrameType) {
	bool outStatus = false;

	if ( !inFrameType.compare(_frameName)  ) outStatus = true;

	return outStatus;
}

bool
DBWorkersStatus::fromJson( const std::string& inJSON, std::string& outError ) {
	std::string jsonFT; //Typ ramki odczytany z JSONa

	try {
		j = json::parse(inJSON);

		jsonFT = j.at("frameType").get<std::string>();

		if ( !frameType.compare(jsonFT) ) {
			try {
				_data = j.at("data").get<WorkersStatusData>();
			} catch (const std::exception& e){
				outError = "DBWorkersStatus::fromJson::data: " + std::string(e.what());
				clear();
				return false;
			}
			_strData = inJSON;
			return true;
		} else {}


	} catch(const std::exception& e) {
		outError = "DBWorkersStatus::fromJson::jsonParse: " + std::string(e.what());
		clear();
	}

	return false;
}

WorkersStatusData&
DBWorkersStatus::getData() {
	return _data;
}

void DBWorkersStatus::showData() {
	std::cout << "Number DbWorkers: " << _data.numberDbWorkers << "\n"
			<< "Statuses: " << _data.wqs.size() << std::endl;
	for (WorkersStatusData::WorkerQueueStatus e : _data.wqs) {
		std::cout << '[' << e.pid << "] = " << e.status << std::endl;
	}
}
std::string
DBWorkersStatus::dataToJson() {
	return _strData;
}
