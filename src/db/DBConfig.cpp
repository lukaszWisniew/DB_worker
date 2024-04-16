/*
 ============================================================================
 Name       :  DBConfig.cpp
 Date       :  24-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "DBConfig.h"

#include <iostream>

DBConfig::DBConfig() {
	_ip     = "undefined";
	_dbName = "undefined";
	_user   = "undefined";
	_pass   = "undefined";
}

DBConfig::~DBConfig() {}

void
DBConfig::setDbName(const std::string &dbName) {
	_dbName = dbName;
}

void
DBConfig::setIp(const std::string &ip) {
	_ip = ip;
}

void
DBConfig::setPass(const std::string &pass) {
	_pass = pass;
}

void
DBConfig::setUser(const std::string &user) {
	_user = user;
}

std::string
DBConfig::getURLConnection() {
	std::string output;

	output="postgresql://"+_user+":"+_pass+"@"+_ip+"/"+_dbName;

	return output;
}

bool
DBConfig::isConfigured() const {
	bool output = false;

	if (("undefined" != _ip) && ("undefined" != _dbName) && ("undefined" != _user) && ("undefined" != _pass)) {
		output = true;
	}

	return output;
}

void
DBConfig::showConfig() {
	std::cout << "[" << "DB_CONNECTION" << "]" << std::endl;
	std::cout << " " << "ADDRESS = " << _ip << std::endl;
	std::cout << " " << "DB_NAME = " << _dbName << std::endl;
	std::cout << " " << "USER    = " << _user << std::endl;
	if ( "undefined" == _pass ) {
		std::cout << " " << "PASS    = " << _pass << std::endl;
	} else {
		std::cout << " " << "PASS    = ********" << std::endl;
	}
	std::cout << "[]" << std::endl;
	std::cout << "" << std::endl;
}
