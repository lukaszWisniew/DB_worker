/*
 ============================================================================
 Name       :  RedisManagerCtx.cpp
 Date       :  27-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "RedisManagerCtx.h"

#include "RedisManagerBus.h"

RedisManagerCtx::RedisManagerCtx()/*:default_tv_toSend (100000), data_tv_ToSend(1000)*/ {

	 redisMgrConnected = false;
	 managerRBConfig   = NULL;
	 redisManagerBus   = NULL;
}

RedisManagerCtx::~RedisManagerCtx() {}

