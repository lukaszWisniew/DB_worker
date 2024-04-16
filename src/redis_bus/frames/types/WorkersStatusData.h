/*
 ============================================================================
 Name       :  WorkersStatusData.h
 Date       :  13-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_FRAMES_TYPES_WORKERSSTATUSDATA_H_
#define REDIS_BUS_FRAMES_TYPES_WORKERSSTATUSDATA_H_

#include <vector>
#include <string>
/**
 * Status wszystkich db_workerów nadzorowanych przez jednego db managera.
 */
class WorkersStatusData final {
public:
	/**
	 * A structure that holds status data in connection with the db_worker PID.
	 */
	struct WorkerQueueStatus {
		/**
		 * db_worker PID.
		 */
		int pid;

		/**
		 * db_worker status.
		 */
		std::string status;
	};

public:

	/**
	 * Number of db_workers.
	 */
	int numberDbWorkers;

	/**
	 * Vector, db_worker status list.
	 */
	std::vector<WorkerQueueStatus> wqs; //Worker Queue Status
public:
	WorkersStatusData();
	virtual ~WorkersStatusData();

	/**
	 * Clearing variables.
	 */
	void clear();
};

#endif /* REDIS_BUS_FRAMES_TYPES_WORKERSSTATUSDATA_H_ */
