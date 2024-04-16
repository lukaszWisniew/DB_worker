/*
 ============================================================================
 Name       :  RedisBusConfig.h
 Date       :  26-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_REDISBUSCONFIG_H_
#define REDIS_BUS_REDISBUSCONFIG_H_

#include <string>

/**
 * The main class that holds the configuration for the redis bus.
 * This class is inherited by other classes to clarify the configuration.
 */
class RedisBusConfig {
public:

	/**
	 * Name of the communication channel in redis.
	 */
	std::string chName;

	/**
	 * The IP address of the redis server.
	 */
	std::string ip;

	/**
	 * Password to connect to redis.
	 */
	std::string password;

	/**
	 * The port number on which the redis service is listening.
	 */
	int port;

public:
	RedisBusConfig();
	virtual ~RedisBusConfig();
	
	/**
	 * Channel name setting.
	 *
	 * @param inChName Channel name
	 */
	void setChName(const std::string &inChName);

	/**
	 * Setting the redis server IP address.
	 *
	 * @param inIP IP address
	 */
	void setIp(const std::string &inIP);

	/**
	 * Setting the redis service port.
	 *
	 * @param inPort Redis service port
	 */
	void setPort(const std::string &inPort);

	/**
	 * Setting the password to connect to redis.
	 *
	 * @param inPassword Password to connect to redis.
	 */
	void setPass(const std::string &inPassword);

	/**
	 * Getting the name of the communication channel in redis.
	 *
	 * @return Channel name
	 */
	std::string getChName();

	/**
	 * Virtual function to show configuration.
	 */
	virtual void showConfig() = 0;

	/**
	 * Virtual function to Checks the correctness of the set configuration.
	 */
	virtual bool isConfigured() const = 0;
};

#endif /* REDIS_BUS_REDISBUSCONFIG_H_ */
