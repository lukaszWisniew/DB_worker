/*
 ============================================================================
 Name       :  DBDataProcessor.h
 Date       :  24-01-2024
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2024 Lukasz Wisniewski
 ============================================================================
 */

#ifndef PROCESSORS_DBDATAPROCESSOR_H_
#define PROCESSORS_DBDATAPROCESSOR_H_

#include "../redis_bus/frames/JsonRedisMainFrame.h"
#include "../db/DBDataMatrix.h"

//--------------------
// Blank
//--------------------
class Context;
//--------------------


/**
 * Managing program state for incoming data queries to the database.
 */
class DBDataProcessor final {
private:
	/**
	 * Global context
	 */
	Context*     _ctx;

	/**
	 * Stores data resulting from a database query.
	 */
	DBDataMatrix _dbDataMatrix;

public:
	DBDataProcessor();
	virtual ~DBDataProcessor();

	/**
	 * Set global context
	 *
	 * @param inCtx pointer to global context
	 */
	void setContext( Context *inCtx );

	/**
	 * Passing the query data to the database.
	 *
	 * @param inJsonRedisFrame frame from redis data bus
	 */
	void takeData( JsonRedisMainFrame* inJsonRedisFrame );



private:

	/**
	 * The engine that determines the program status and database query steps.
	 *
	 * @param inFrame frame from redis data bus
	 */
	void _engine(DataDBRequest *inFrame);

	/**
	 * Setting the appropriate status.
	 * Sending a query to the database query queue.
	 *
	 * @param inFrame frame from redis data bus
	 */
	void _setStatusAndSendQuery(DataDBRequest *inFrame);

	/**
	 * Setting the program status after querying the database.
	 */
	void _statusAfterResponse();

	/**
	 * Forwarding to query the database for data.
	 * Sending feedback to the redis data bus.
	 *
	 * @param inFrame frame from redis data bus
	 */
	void _doQuery (DataDBRequest *inFrame);

	/**
	 * Sending feedback to the redis data bus.
	 *
	 * The original query frame passed to read the sender's metadata.
	 *
	 * @param inFrame frame from redis data bus
	 */
	void _transferResponse(DataDBRequest *inFrame);

};

#endif /* PROCESSORS_DBDATAPROCESSOR_H_ */
