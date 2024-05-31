/*
 ============================================================================
 Name       :  RedisDataCtx.h
 Date       :  27-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_DATA_REDISDATACTX_H_
#define REDIS_BUS_DATA_REDISDATACTX_H_

#include "../RedisCtx.h"

#include "../config/DataRBConfig.h"
#include "../config/QueueStatusType.h"

//--------------------
// Blank
//--------------------
class RedisDataBus;
//--------------------

/**
 * Description of the context that holds the variables necessary
 * to handle queries from the redis bus to the database.
 *
 * @see RedisCtx
 */
class RedisDataCtx final : public RedisCtx {
public:

	/**
	 * The flag describing whether db_worker is connected to redis.
	 */
	bool          redisDataConnected;

	/**
	 * Configuring the database query bus.
	 * Configuration comes from configuration file.
	 */
	DataRBConfig* dataRBConfig;

	/**
	 * The class responsible for communicating with the redis data bus
	 *  for queries and responses to and from the database.
	 */
	RedisDataBus* redisDataBus;

	/**
	 * db_worker status.
	 */
	QueueStatus::Type dataQueueStatus;

	/**
	 * db_worker PID.
	 */
	pid_t pid;

	/**
	 * Time value frequency of sending db_worker status to the manager bus.
	 *
	 * @see libevent
	 */
	struct timeval tv_statusInterval;

	/**
	 * Event for f sending db_worker status to the manager bus.
	 *
	 * @see libevent
	 */
	struct event* statusResend_ev;

public:
	RedisDataCtx();
	virtual ~RedisDataCtx();
};

#endif /* REDIS_BUS_DATA_REDISDATACTX_H_ */
