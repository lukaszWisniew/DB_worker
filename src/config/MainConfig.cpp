/*
 ============================================================================
 Name       :  MainConfig.cpp
 Date       :  11-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "MainConfig.h"

#include <iostream>

MainConfig::MainConfig() {
	_logEnable = false;
}

MainConfig::~MainConfig() {}

bool
MainConfig::isLogEnable() const {
	return _logEnable;
}

void
MainConfig::setLogEnable(bool logEnable) {
	_logEnable = logEnable;
}

void
MainConfig::showConfig() {
	std::cout << "[" << "MAIN" << "]" << std::endl;
	std::cout << " " << "LOG_ENABLE = " << (_logEnable ? "yes" : "no") << std::endl;
	std::cout << "[]" << std::endl;
	std::cout << "" << std::endl;
}
