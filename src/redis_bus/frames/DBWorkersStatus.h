/*
 ============================================================================
 Name       :  DBWorkersStatus.h
 Date       :  12-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_FRAMES_DBWORKERSSTATUS_H_
#define REDIS_BUS_FRAMES_DBWORKERSSTATUS_H_

#include "JsonRedisMainFrame.h"
#include "types/WorkersStatusData.h"
#include "../config/QueueStatusType.h"

class DBWorkersStatus final : public JsonRedisMainFrame {
private:
	/**
	 * Full frame in json format.
	 */
	std::string        _strData;

	/**
	 * Frame name.
	 */
	static std::string _frameName;

	/**
	 * db_worker statuses.
	 */
	WorkersStatusData  _data;
public:
	DBWorkersStatus();
	virtual ~DBWorkersStatus();

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
     * db_worker statuses with no upload queue parameters.
     *
     * @return Frame data
	 */
	WorkersStatusData& getData();

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
	 * Downloading the frame content in sring/json format.
	 *
	 * @return Frame in json format.
	 */
	std::string dataToJson();
};

#endif /* REDIS_BUS_FRAMES_DBWORKERSSTATUS_H_ */
