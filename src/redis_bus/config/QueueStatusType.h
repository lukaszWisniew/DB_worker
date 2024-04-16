/*
 ============================================================================
 Name       :  QueueStatusType.h
 Date       :  30-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
*/

#include <string>


#ifndef REDIS_BUS_CONFIG_QUEUESTATUSTYPE_H_
#define REDIS_BUS_CONFIG_QUEUESTATUSTYPE_H_

/**
 * A class that wraps the enum class.
 * Its task is to provide the enum itself
 * as well as format changes to string and from string to enum.
 */
class QueueStatus {
public:
	/**
	 * The enum class describes the status of the db_worker.
	 */
	enum class Type {
		OPEN, //Ready to accept a database query.
		STAND_BY, //It is waiting its turn to be processed, to open status.
		CLOSED, //Closed, supporting inquiry.
		START, //Status after program start, db_worker blocked.
		BLOCKED, //Blocked by the manager or the number of queries to be handled exceeded the maximum.
		none //Technical and initialization status.
	};

	/**
	 * Function that converts enum to string.
	 *
	 * @param inQueueStatusType The enum value to convert.
	 *
	 * @return Enum name, string value.
	 */
	static std::string typeToString( QueueStatus::Type inQueueStatusType );

	/**
	 * The function converts a string to an enum.
     * If the enum is not found, none is returned.
     *
     * @param inQueueStatusType Parameter describing the status of the db_worker, type string.
     *
     * @return An enum object representing the value passed as a string.
	 */
	static Type typeFromString( std::string inQueueStatusType );

};

#endif /* REDIS_BUS_CONFIG_QUEUESTATUSTYPE_H_ */
