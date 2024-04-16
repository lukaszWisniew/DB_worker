/*
 ============================================================================
 Name       :  DataRBConfig.cpp
 Date       :  26-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "DataRBConfig.h"

#include <iostream>

DataRBConfig::DataRBConfig() {
	_domain = "undefined";
}

DataRBConfig::~DataRBConfig() {

}

void
DataRBConfig::setDomain(const std::string &inDomain) {
	_domain = inDomain;
}

std::string
DataRBConfig::getDomain() {
	return _domain;
}

void
DataRBConfig::showConfig() {
	std::cout << "[" << "DATA_BUS" << "]" << std::endl;
	std::cout << " " << "ADDRESS = " << ip << std::endl;
	std::cout << " " << "PORT = " << port << std::endl;
	std::cout << " " << "CHANNEL_NAME = " << chName << std::endl;
	std::cout << " " << "DOMAIN = " << _domain << std::endl;
	std::cout << " " << "PASS = ********" << std::endl;
	std::cout << "[]" << std::endl;
	std::cout << "" << std::endl;
}

bool
DataRBConfig::isConfigured() const {
	bool output = false;

	if (("undefined" != ip) && ("undefined" != password ) && ("undefined" != chName) && ("undefined" != _domain) && ((port >= 1024) && (port <= 65000))) {
		output = true;
	}

	return output;
}
