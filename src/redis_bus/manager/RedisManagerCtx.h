/*
 ============================================================================
 Name       :  RedisManagerCtx.h
 Date       :  27-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_MANAGER_REDISMANAGERCTX_H_
#define REDIS_BUS_MANAGER_REDISMANAGERCTX_H_

#include "../RedisCtx.h"

#include "../config/ManagerRBConfig.h"


//--------------------
// Blank
//--------------------
class RedisManagerBus;
//--------------------

/**
 * Description of the context that holds the variables necessary
 * to handle queries from the redis management bus.
 *
 * @see RedisCtx
 */
class RedisManagerCtx final : public RedisCtx {
public:

	/**
	 * The flag describing whether db_worker is connected to redis.
	 */
	bool             redisMgrConnected;

	/**
	 * Configuration for the bus to exchange the state of db_workers.
	 * Configuration comes from configuration file.
	 *
	 * @see ManagerRBConfig
	 */
	ManagerRBConfig* managerRBConfig;

	/**
	 * Communicating with the Redis management bus,
	 * Responsible for receiving information about the statuses of other db_workers
	 * and sending information about the status of this db_worker.
	 *
	 * @see RedisManagerBus
	 */
	RedisManagerBus* redisManagerBus;

public:
	RedisManagerCtx();
	virtual ~RedisManagerCtx();
};

#endif /* REDIS_BUS_MANAGER_REDISMANAGERCTX_H_ */
