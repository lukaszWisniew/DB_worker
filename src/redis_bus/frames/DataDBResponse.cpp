/*
 ============================================================================
 Name       :  DataDBResponse.cpp
 Date       :  25-01-2024
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2024 Lukasz Wisniewski
 ============================================================================
 */

#include "DataDBResponse.h"

#include <iostream>

std::string  DataDBResponse::_frameName = "dataDBResponse";

DataDBResponse::DataDBResponse() {
	frameType = _frameName;
	clear();
}

DataDBResponse::~DataDBResponse() {}

void
DataDBResponse::setMetaDataFrame( DataDBRequest *inFrame ) {
	pid = inFrame->pid;
	queryName = inFrame->data.queryName;
}

void
DataDBResponse::setData( std::vector<std::vector<std::string>> &inData ) {
	data = inData;
}


std::string
DataDBResponse::toJson() {
	j = (*this);
	return j.dump();
}

bool
DataDBResponse::fromJson( const std::string& inJSON, std::string& outError ) {
	return false;
}

void
DataDBResponse::showData() {
	std::cout << "VERSION:" << version << "\n"
			  << "FRAME NAME: " << frameType << "\n"
	          << "PID: " << pid << "\n"
			  << "DATA: " << "\n"
			  << " QUERY NAME: " << queryName << "\n";
	for (auto r : data ) {
		for (auto c : r ) std::cout << "  " << c << " ";
		std::cout << "\n";
	}
	std::cout << std::endl;
}

void
DataDBResponse::clear() {
	pid = -1;
	queryName = "none";

	for (auto r : data ) r.clear();
	data.clear();
}
