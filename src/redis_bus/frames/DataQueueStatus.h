/*
 ============================================================================
 Name       :  DataQueueStatus.h
 Date       :  30-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_FRAMES_DATAQUEUESTATUS_H_
#define REDIS_BUS_FRAMES_DATAQUEUESTATUS_H_

#include <string>

#include "JsonRedisMainFrame.h"


#include "../manager/RedisManagerCtx.h"
#include "types/QueueData.h"


/**
 * The class represents a frame with information about the db_worker status.
 * The frame is received and sent to the management bus.
 */
class DataQueueStatus final : public JsonRedisMainFrame {
public:
	/**
	 * db_worker pid.
	 */
	pid_t pid;

	/**
	 * db_worker status.
	 */
	QueueData data;
private:

	/**
	 * Full frame in json format.
	 */
	std::string _strData;

	/**
	 * Frame name.
	 */
	static std::string _frameName;
public:
	DataQueueStatus();
	virtual ~DataQueueStatus();

	/**
	 * Getting the db_worker status in the form of a string.
	 *
	 * @return db_worker status
	 */
	std::string getQueueStatusS();

	/**
	 * Checking whether the downloaded data is in json format and contains the given frame name.
	 *
	 * @param inJSON Data from management bus.
	 *
	 * @return True if the data represents the expected frame.
	 */
	static bool itIsJson(const std::string& inJSON);

	/**
	 * Check whether the passed frame type is a frame type supported by this class.
	 *
	 * @param inFrameType Frame type
	 *
	 * @return True if the frametype passed is a frametype supported by this class.
	 */
	static bool itIsString(const std::string& inFrameType);

	/**
	 * Downloading the frame content in sring/json format.
	 *
	 * @return Frame in json format.
	 */
	std::string toJson();

	/**
	 * Converting the frame from json to a data structure.
	 *
	 * @param inJSON Frame in json format.
	 * @param outError outError Error content.
	 *
	 * return True if the data can be converted.
	 */
	bool fromJson( const std::string& inJSON, std::string& outError );

	/**
	 * Downloading data from the frame.
     * db_worker status with sending queue parameters.
     *
     * @return Frame data
	 */
	QueueData& getData();

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
};

#endif /* REDIS_BUS_FRAMES_DATAQUEUESTATUS_H_ */
