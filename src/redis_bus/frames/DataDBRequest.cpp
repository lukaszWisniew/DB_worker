/*
 ============================================================================
 Name       :  DataDBRequest.cpp
 Date       :  22-01-2024
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2024 Lukasz Wisniewski
 ============================================================================
 */

#include "DataDBRequest.h"

#include <bits/exception.h>
#include <iostream>

#include "json.hpp"


std::string DataDBRequest::_frameName = "dataDBRequest";

DataDBRequest::DataDBRequest() {
	frameType = _frameName;

	clear();

}

DataDBRequest::~DataDBRequest() {}

bool
DataDBRequest::itIsJson(const std::string& inJSON) {
	bool outStatus = false;
	std::string jsonFT; //Typ ramki odczytany z JSONa
	json j = json::parse(inJSON);

	if ( j.find("frameType") != j.end() ) {

		jsonFT = j.at("frameType").get<std::string>();

		if ( !jsonFT.compare(_frameName) ) {
			outStatus = true;
		}
	}

	return outStatus;
}

bool
DataDBRequest::itIsMyDomain(const std::string& inJSON, const std::string inDomain) {
	bool outStatus = false;
	std::string jsonDo; //Nazwa domeny ramki
	json j = json::parse(inJSON);

	if ( j.find("domain") != j.end() ) {

		jsonDo = j.at("domain").get<std::string>();

		if ( !jsonDo.compare(inDomain) ) {
			outStatus = true;
		}
	}

	return outStatus;
}

bool
DataDBRequest::fromJson(const std::string &inJSON, std::string &outError) {

	std::string jsonFT; //Typ ramki odczytany z JSONa

	_strData.clear();

	try {

		j = json::parse(inJSON);

		if ( j.find("frameType") != j.end() ) {

			jsonFT = j.at("frameType").get<std::string>();

			if ( !frameType.compare(jsonFT) ) {
				try {
					pid = j.at("/pid"_json_pointer).get<int>();
					data = j.at("data").get<DBRequestData>();
					if ( "" == data.queryName) {
						outError = "DataDBRequest::fromJson::data: No query name.";
						clear();
						return false;
					}
				} catch (const std::exception& e){
					outError = "DataDBRequest::fromJson::data: " + std::string(e.what());
					clear();
					return false;
				}
				_strData = inJSON;
				return true;
			} else {

			}
		} else {
			return false;
		}

	} catch(const std::exception& e) {
		outError = "DataDBRequest::fromJson::jsonParse: " + std::string(e.what());
		clear();
	}


	return false;

}

void
DataDBRequest::clear() {
	pid = -1;
	data.clear();
}

void
DataDBRequest::showData() {
	std::cout << "PID: " << pid << "\n"
			  << std::endl;
}

std::string
DataDBRequest::dataToJson() {
	return _strData;
}
