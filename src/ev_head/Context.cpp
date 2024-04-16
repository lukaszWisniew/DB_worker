/*
 ============================================================================
 Name       :  Context.cpp
 Date       :  04-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "Context.h"

#include <iostream>

#include "../config/Configuration.h"

Context::Context() {
	pid = getpid();
}

Context::~Context() {}

void
Context::setBase(event_base *inBase) {
	EV_context::setBase(inBase);

	redisManagerBus.setContext(this);
	redisDataBus.setContext(this);
	managerProcessor.setContext(this);
	dbDataProcessor.setContext(this);
}

void
Context::setConfiguration(lw_lib::IConfiguration *inConfiguration) {
	EV_context::setConfiguration(inConfiguration);

	Configuration *conf = dynamic_cast<Configuration*>(inConfiguration);

	log.setLogConf(&(conf->logConf));
	dbDispatcher.setConfiguration(&(conf->dbConfig), &(log));
}

void
Context::startAutoService() {
	redisDataBus.startRedisToSend();
}


lw_lib::Log*
Context::getLog() {
	return &log;
}


