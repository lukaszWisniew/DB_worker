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
};

#endif /* REDIS_BUS_MANAGERRBCONFIG_H_ */
