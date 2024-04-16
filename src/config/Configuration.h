/*
 ============================================================================
 Name       :  Configuration.h
 Date       :  04-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <config/IConfiguration.h>
#include <log/LogConf.h>
#include <log/LogLevel.h>

#include "MainConfig.h"
#include "../db/DBConfig.h"
#include "../redis_bus/config/DataRBConfig.h"
#include "../redis_bus/config/ManagerRBConfig.h"

/**
 * A class that reads the configuration file and holds the full program configuration.
 */
class Configuration final : public lw_lib::IConfiguration {
public:
	/**
	 * Log configuration
	 *
	 * See lw_lib::LogConf.
	 */
	lw_lib::LogConf logConf;

	/**
	 * Main configuration of software.
	 * Command line switches and more
	 * See MainConfig
	 */
	MainConfig      mainConfig;

	/**
	 * Configuring the connection to the Postgres database.
	 */
	DBConfig        dbConfig;

	/**
	 * Configuration the connection to the Redis data bus.
	 */
	DataRBConfig    dataRBConfig;

	/**
	 * Configuration the connection to the Redis manager bus.
	 */
	ManagerRBConfig managerRBConfig;
public:
	Configuration();
	virtual ~Configuration();

	/** Reading the configuration file and setting the appropriate program parameters. */
	virtual void readConfigFile();

	/** Printing the full program configuration. */
	virtual void showConfig();

	/**
	 * Returns a reference to configuration the connection to the Redis manager bus.
	 *
	 * @return reference to managerRBConfig
	 */
	ManagerRBConfig* getManagerRBConfig();

	/**
	 * Returns a reference to configuration the connection to the Redis data bus.
	 *
	 * @return reference to dataRBConfig
	 */
	DataRBConfig* getDataRBConfig();
};

#endif /* CONFIGURATION_H_ */
