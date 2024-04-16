/*
 ============================================================================
 Name       :  DataRBConfig.h
 Date       :  26-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_DATARBCONFIG_H_
#define REDIS_BUS_DATARBCONFIG_H_

#include "RedisBusConfig.h"

#include <string>

/**
 * Configuring the database query bus.
 * Configuration comes from configuration file.
 *
 * @See RedisBusConfig
 */

class DataRBConfig final : public RedisBusConfig {
private:
	/**
	 * A name that describes the query group.
	 * Parts of the system are about inquiry.
	 */
	std::string _domain;
public:
	DataRBConfig();
	virtual ~DataRBConfig();

	/**
	 * Domain name setting.
	 *
	 * @param inDomain Domain name
	 */
	void setDomain(const std::string &inDomain);

	/**
	 * Get the domain name.
	 *
	 * @return domain name
	 */
	std::string getDomain();


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

#endif /* REDIS_BUS_DATARBCONFIG_H_ */
