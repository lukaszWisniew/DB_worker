/*
 ============================================================================
 Name       :  DataDBRequest.h
 Date       :  22-01-2024
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2024 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_FRAMES_DATADBREQUEST_H_
#define REDIS_BUS_FRAMES_DATADBREQUEST_H_

#include <string>

#include "JsonRedisMainFrame.h"
#include "types/DBRequestData.h"

/**
 * A class representing a query from redis to the database.
 *
 * @see JsonRedisMainFrame
 */
class DataDBRequest final : public JsonRedisMainFrame {
public:
	/**
	 * PID of the requesting process.
	 */
	pid_t pid;

	/**
	 * Query data from the redis bus.
	 * @see DBRequestData
	 */
	DBRequestData data;
private:

	/**
	 * Frame name, header.
	 */
	static std::string _frameName;

	/**
	 * Frame in string, json format.
	 */
	std::string _strData;



public:
	DataDBRequest();
	virtual ~DataDBRequest();

	DataDBRequest& operator=(const DataDBRequest& other);

	/**
	 * Checking whether the transferred data is in json format and contains the appropriate frame name.
	 *
	 * @param inJSON Data from redis in json format
	 *
	 * @return True if it's json and contains the appropriate frame name.
	 */
	static bool itIsJson(const std::string& inJSON);

	/**
	 * Checks whether the domain in the frame is supported by this db_worker.
	 *
	 * @param inJSON Data from redis in json format
	 * @param inDomain The domain name from the incoming frame.
	 *
	 * @return True if the domain in the frame is handled by db_worker.
	 */
	static bool itIsMyDomain(const std::string& inJSON, const std::string inDomain);

	/**
	 * Convert from json(string) to a DBRequestData data structure.
	 *
	 * @see DBRequestData
	 *
	 * @param inJSON Data from redis in json format
	 * @param outError Error content.
	 *
	 * return True if the data can be converted.
	 */
	bool fromJson( const std::string& inJSON, std::string& outError );

	/**
	 * Clearing variables.
	 */
	void clear();

	/**
	 * Show configuration.
	 *
	 * Send on stdout information about configuration.
	 */
	void showData();

	/**
	 * Downloading the frame, query in json format, original.
	 *
	 * return Frame body.
	 */
	std::string dataToJson();
};

#endif /* REDIS_BUS_FRAMES_DATADBREQUEST_H_ */
