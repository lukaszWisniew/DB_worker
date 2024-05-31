/*
 ============================================================================
 Name       :  QueueData.h
 Date       :  12-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_FRAMES_TYPES_QUEUEDATA_H_
#define REDIS_BUS_FRAMES_TYPES_QUEUEDATA_H_

#include "../../config/QueueStatusType.h"


/**
 * Description of the db_worker status along with
 * the parameters of the queue for sending data to the redis bus.
 */
class QueueData final {
public:
	/**
	 * db_worker status
	 */
	QueueStatus::Type dataQueueStatus;

	/**
	 * Maximum queue length.
	 */
	//int             maxSize;

	/**
	 * The value below which unlocks the acceptance of new queries.
	 */
	//int             openSize;

	/**
	 * Current queue length.
	 */
	//int             size;
public:
	QueueData();
	virtual ~QueueData();

	/**
	 * Clearing variables.
	 */
	void clear();
};

#endif /* REDIS_BUS_FRAMES_TYPES_QUEUEDATA_H_ */
