/*
 ============================================================================
 Name       :  Context.h
 Date       :  04-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <unistd.h>

#include <ev_main/EVcontext.h>

#include <log/Log.h>
#include <sw/redis++/redis++.h>

#include "../db/DBDispatcher.h"
#include "../redis_bus/manager/RedisManagerBus.h"
#include "../redis_bus/data/RedisDataBus.h"
#include "../processors/ManagerProcessor.h"
#include "../processors/DBDataProcessor.h"

/**
 * The main class that ties the entire program together.
 * It contains objects and methods necessary for the logic of the entire program.
 * The class implements the EV_context interface
 *
 * @see lw_lib::EV_context
 */
class Context final : public lw_lib::EV_context {
public:

	/**
	 * Logging engine.
	 */
	lw_lib::Log  log;

	/**
	 * Managing the connection to the Postgresql.
	 */
	DBDispatcher dbDispatcher;

	/**
	 * Managing the connection to the redis management bus.
	 */
	RedisManagerBus redisManagerBus;

	/**
	 * Managing the connection to the redis data bus.
	 */
	RedisDataBus redisDataBus;

	/**
	 * Analyzes frames from the manager bus.
	 * Manages the status of the database query queue.
	 */
	ManagerProcessor managerProcessor;

	/**
	 * Analyzes frames from the data bus.
	 * Manages the processing of data queries to the database.
	 */
	DBDataProcessor dbDataProcessor;

	/**
	 * Process PID number.
	 */
	pid_t pid;

public:
	Context();
	virtual ~Context();

	/**
	 * Event_base setting for libevent2
	 *
	 * @param inBase Structure to hold information and state for a Libevent dispatch loop.
	 *
	 * @see libevent2 documentation.
	 */
	void setBase( event_base *inBase );

	/**
	 * Passing an object holding the full configuration.
	 *
	 * @param inConfiguration An object of a class implementing IConfiguration.
	 */
	void setConfiguration( lw_lib::IConfiguration *inConfiguration );

	/**
     * A method that starts background processes.
     */
	void startAutoService();


	/**
	 * Getting a pointer to the object that handles logging.
	 *
	 * @return pointer to the log object.
	 */
	lw_lib::Log* getLog();
};

#endif /* CONTEXT_H_ */
