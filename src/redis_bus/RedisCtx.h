/*
 ============================================================================
 Name       :  RedisCtx.h
 Date       :  27-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDISCTX_H_
#define REDISCTX_H_

#include <event2/event.h>
#include <queue>
#include <string>

#include <log/Log.h>

/**
 * A class that is inherited by other classes,
 *  responsible for contexts related to communication with redis.
 */
class RedisCtx {
public:
	/**
	 * Another context usually the main one.
	 */
	void*            external_ctx;

	/**
	 *  event_base structure holds a set of events and can poll to determine which events are active
	 *
	 *  @see libevent
	 */
	event_base*      base;

	/**
	 * Log processor.
	 *
	 * @see lw_lib::Log
	 */
	lw_lib::Log*     log;

	/**
	 * Time value for reconnect to redis
	 *
	 * @see libevent
	 */
	struct timeval tv_reconnect;

	/**
	 * Event for reconnect to redis.
	 *
	 * @see libevent
	 */
	struct event* redisReconnect_ev;

public:
	RedisCtx();
	virtual ~RedisCtx();
};

#endif /* REDISCTX_H_ */
