/*
 ============================================================================
 Name       :  RedisBusConfig.cpp
 Date       :  26-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "RedisBusConfig.h"

#include <misc/Procedures.h>

RedisBusConfig::RedisBusConfig() {
	chName = "undefined";
	ip = "undefined";
	password = "undefined";
	port = -1;

}

RedisBusConfig::~RedisBusConfig() {}

void
RedisBusConfig::setChName(const std::string &inChName) {
	chName = inChName;
}

void
RedisBusConfig::setIp(const std::string &inIP) {
	ip = inIP;
}

void
RedisBusConfig::setPort(const std::string &inPort) {
	port = lw_lib::stringTo<int>(inPort);
}

void
RedisBusConfig::setPass(const std::string &inPassword) {
	password = inPassword;
}

std::string
RedisBusConfig::getChName() {
	return chName;
}


