/*
 ============================================================================
 Name       :  RedisCtx.cpp
 Date       :  27-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "RedisCtx.h"

RedisCtx::RedisCtx() {

	 external_ctx = NULL;
	 base = NULL;

	 log = NULL;

	 tv_reconnect.tv_sec = 1;
	 tv_reconnect.tv_usec = 0;

	 redisReconnect_ev = NULL;
}

RedisCtx::~RedisCtx() {}

