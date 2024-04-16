/*
 ============================================================================
 Name       :  DataDBResponse.h
 Date       :  25-01-2024
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2024 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_FRAMES_DATADBRESPONSE_H_
#define REDIS_BUS_FRAMES_DATADBRESPONSE_H_

#include "JsonRedisMainFrame.h"

#include <vector>
#include <string>

#include "../frames/DataDBResponse.h"
#include "../frames/DataDBRequest.h"

/**
 * The class representing the answer to a database question.
 *
 * @see JsonRedisMainFrame
 */
class DataDBResponse final : public JsonRedisMainFrame {
private:
	/**
	 * Frame name, header.
	 */
	static std::string _frameName;
public:

	/**
	 * Pid of the process to which we are sending data.
	 */
	pid_t pid;

	/**
	 * Query name
	 */
	std::string queryName;

	/**
	 * Data returned by the database.
	 */
	std::vector<std::vector<std::string>> data;

public:
	DataDBResponse();
	virtual ~DataDBResponse();

	/**
	 * Response metadata setting: pid, query name.
	 *
	 * @param inFrame Data from request frame
	 */
	void setMetaDataFrame( DataDBRequest *inFrame );

	/**
	 * Response data from data base
	 *
	 * @param inData data from DB.
	 */
	void setData( std::vector<std::vector<std::string>> &inData );

	/**
	 * Blank
	 */
	std::string toJson();

	/**
	 * Blank
	 */
	bool fromJson( const std::string& inJSON, std::string& outError );

	/**
	 * Show configuration.
	 *
	 * Send on stdout information about configuration.
	 */
	void showData();

	/**
	 * Clearing variables.
	 */
	void clear();
};

#endif /* REDIS_BUS_FRAMES_DATADBRESPONSE_H_ */
