/*
 ============================================================================
 Name       :  ManagerRBConfig.cpp
 Date       :  26-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "ManagerRBConfig.h"

#include <iostream>

ManagerRBConfig::ManagerRBConfig() {}

ManagerRBConfig::~ManagerRBConfig() {}


void
ManagerRBConfig::showConfig() {

	std::cout << "[" << "DB_MANAGER_BUS" << "]" << std::endl;
	std::cout << " " << "ADDRESS = " << ip << std::endl;
	std::cout << " " << "PORT = " << port << std::endl;
	std::cout << " " << "CHANNEL_NAME = " << chName << std::endl;
	std::cout << " " << "PASS = ******** " << std::endl;
	std::cout << " " << "STATUS_INTERVAL = " << repeat_status_interval << std::endl;
	std::cout << "[]" << std::endl;
	std::cout << "" << std::endl;
}

bool
ManagerRBConfig::isConfigured() const {
	bool output = false;

	if (("undefined" != ip) && ("undefined" != password ) && ("undefined" != chName) && ((port >= 1024) && (port <= 65000))) {
		output = true;
	}

	return output;
}

void
ManagerRBConfig::setStatusInterval(int inStatusInterval) {
	if ( inStatusInterval > 0 ) {
		repeat_status_interval = inStatusInterval;
	}
}

int
ManagerRBConfig::getStatusInterval() {
	return repeat_status_interval;
}
