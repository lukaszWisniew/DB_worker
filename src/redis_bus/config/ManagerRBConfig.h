/*
 ============================================================================
 Name       :  ManagerRBConfig.h
 Date       :  26-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_MANAGERRBCONFIG_H_
#define REDIS_BUS_MANAGERRBCONFIG_H_

#include "RedisBusConfig.h"


/**
 * Configuration for the bus to exchange the state of db_workers.
 * Configuration comes from configuration file.
 *
 * @See RedisBusConfig
 */
class ManagerRBConfig final : public RedisBusConfig {
private:
	/**
	 * Frequency of sending db_worker status to the manager bus.
	 */
	int repeat_status_interval = 1;
public:
	ManagerRBConfig();
	virtual ~ManagerRBConfig();

	/**
	 * Show configuration.
	 *
	 * Send on stdout information about configuration.
	 */
	void showConfig();

	/**
	 * Checks the correctness of the set configuration.
	 */
	bool isConfigured() const;

	/**
	 * Setting the frequency of sending the db_worker status to the manager bus.
	 *
	 * @param inStatusInterval interval in seconds
	 */
	void setStatusInterval( int inStatusInterval );

	/**
	 * Getting the frequency of sending the db_worker status to the manager bus.
	 *
	 * @return interval in seconds
	 */
	int getStatusInterval();
};

#endif /* REDIS_BUS_MANAGERRBCONFIG_H_ */
