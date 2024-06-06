/*
 ============================================================================
 Name       :  ManagerProcessor.h
 Date       :  12-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef PROCESSORS_MANAGERPROCESSOR_H_
#define PROCESSORS_MANAGERPROCESSOR_H_

#include <map>

#include "../redis_bus/frames/JsonRedisMainFrame.h"
#include "../redis_bus/config/QueueStatusType.h"

//--------------------
// Blank
//--------------------
class Context;
//--------------------

/**
 * Managing program status with respect to the status of other DB_workers.
 */

class ManagerProcessor final {
private:
	/**
	 * Status map of individual db_workers.
	 * <PID, DB_Worker's status>
	 */
	std::map<int, QueueStatus::Type> _pidStatus;

	/**
	 * Previous status of the db_worker connected to the management bus.
	 * <PID, old DB_Worker's status>
	 */
	std::pair<int,QueueStatus::Type> _old_status;

	/**
	 * Global context
	 */
	Context *_ctx;

	/**
	 * Status of querying data from the database from the data bus.
	 *
	 *  1 - new request from the data bus
	 *  0 - processed new request from the data bus
	 * -1 - new frame is not set, query from the data bus
	 */
	int _newFrameFromBus;

public:
	ManagerProcessor();
	virtual ~ManagerProcessor();

	/**
	 * Set global context
	 *
	 * @param inCtx pointer to global context
	 */
	void setContext( Context *inCtx );

	/**
	 * Passing a frame with the status or statuses of db_workers.
	 *
	 * @param inJsonRedisFrame frame with frame with the status or statuses of db_workers.
	 */
	void takeData( JsonRedisMainFrame* inJsonRedisFrame );

	/**
	 * Checking if there is one db_worker instance connected to the management bus.
	 *
	 * @return true if is alone.
	 */
	bool imAlone();

	/**
	 * Public adapter for setting db_worker status to OPEN.
	 */
	void setQueueStatus_OPEN();

	/**
	 * Public adapter for setting db_worker status to CLOSED.
	 */
	void setQueueStatus_CLOSED();

	/**
	 * Public adapter for setting db_worker status to BLOCKED.
	 */
	void setQueueStatus_BLOCKED();

	/**
	 * Public adapter for setting db_worker status to STAND_BY.
	 */
	void setQueueStatus_STAND_BY();

	/**
	 * Setting the appropriate status after making a request to the database.
	 */
	void setStatusAfterRequestDB();

	/**
	 * Public adapter for setting status: arrival of a new frame on the database query bus.
	 */
	void setTrueNewFrameFromBus();

	/**
	 * Public adapter for setting status: processing a frame from the query bus to the database.
	 */
	void setProcessedNewFrameFromBus();

	/**
	 * Public adapter for setting status: no frame from query bus to database.
	 */
	void setNotSetNewFrameFromBus();

	/**
	 * Whether another process is already processing the query.
	 *
	 * @return tru if another process is already processing the query to data base.
	 */
	bool processedByOther();

	/**
	 * Emergency frame takeover for processing.
	 *
	 * @return true if it's our turn for an emergency frame capture.
	 */
	bool emergencyAmINext();

private:
	/**
	 * Set db_worker statuses.
	 * Starts the engine.
	 *
	 * @see _engine
	 *
	 * @param inWorkersStatusData DB_worker status list.
	 */
	void _setStatuses( WorkersStatusData& inWorkersStatusData );

	/**
	 * Set db_worker status
	 * Starts the engine.
	 *
	 * @see _engine
	 *
	 * @param inDataQueueStatusData DB_worker status.
	 */
	void _setStatus( QueueData& inDataQueueStatusData, int inPid );

	/**
	 * Engine, algorithm that determines the program status in the context of the status of other db_workers.
	 */
	void _engine();

	/**
	 * Engine, algorithm that determines the program status in the context of the status of other db_workers.
	 */
	void _engine_m();

	/**
	 * Getting the pid of the first db_worker with OPEN status.
	 *
	 * @param outPid PID number of the found DB_worker. 0 if not found.
	 *
	 * @return true if found,
	 */
	bool _getOpenPID(int &outPid);

	/**
	 * Getting the pid of the first db_worker with CLOSED status.
	 *
	 * @param outPid PID number of the found DB_worker. 0 if not found.
	 *
	 * @return true if found,
	 */
	bool _getClosedPID(int &outPid);

	/**
	 * Getting the pid of the first db_worker with BLOCKED status.
	 *
	 * @param outPid PID number of the found DB_worker. 0 if not found.
	 *
	 * @return true if found,
	 */
	bool _getBlockedPID(int &outPid);

	/**
	 * Checking if our PID is next.
	 *
	 * @param inPid Pid number of db_worker
	 *
	 * @return true if we are next.
	 */
	bool _amInext( int &inPid );

	/**
	 * Find the next PID from input PID.
	 *
	 * @param inPid Pid number of db_worker
	 *
	 * @return Pid of the next db_worker.
	 */
	int  _findNext( int &inPid );

	/**
	 * After receiving the return frame from the manager,
	 * check whether we are the first to set the status to OPEN.
	 *
	 * @return true if we are to set the status to OPEN.
	 */

	bool _iMFirst();

	/**
	 * Clear _old_status parameter.
	 */
	void _clearOldStatus();

	/**
	 * Setting db_worker status to OPEN.
	 */
	void _setQueueStatus_OPEN();

	/**
	 * Setting db_worker status to CLOSED.
	 */
	void _setQueueStatus_CLOSED();

	/**
	 * Setting db_worker status to BLOCKED.
	 */
	void _setQueueStatus_BLOCKED();

	/**
	 * Setting db_worker status to STAND_BY.
	 */
	void _setQueueStatus_STAND_BY();

	/**
	 * Setting status: arrival of a new frame on the database query bus.
	 */
	void _setTrueNewFrameFromBus();

	/**
	 * Setting status: processing a frame from the query bus to the database.
	 */
	void _setProcessedNewFrameFromBus();

	/**
	 * Setting status: no frame from query bus to database.
	 */
	void _setNotSetNewFrameFromBus();
};

#endif /* PROCESSORS_MANAGERPROCESSOR_H_ */
