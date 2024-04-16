/*
 ============================================================================
 Name       :  RedisDataCtx.cpp
 Date       :  27-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "RedisDataCtx.h"

RedisDataCtx::RedisDataCtx()/*:default_tv_toSend (100000), data_tv_ToSend(1000)*/ {

	redisDataConnected = false;
	dataRBConfig       = NULL;
	redisDataBus       = NULL;

	dataQueueStatus = QueueStatus::Type::START;

	pid = 0;
}

RedisDataCtx::~RedisDataCtx() {}

