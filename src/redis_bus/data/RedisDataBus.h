/*
 ============================================================================
 Name       :  RedisDataBus.h
 Date       :  17-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_DATA_REDISDATABUS_H_
#define REDIS_BUS_DATA_REDISDATABUS_H_

#include <queue>
#include <string>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>

#include <log/Log.h>


#include "../../config/Configuration.h"
#include "../frames/DataQueueStatus.h"
#include "../frames/DataDBResponse.h"
#include "../../db/DBDataMatrix.h"
#include "RedisDataCtx.h"




//--------------------
// Blank
//--------------------
class Context;
//--------------------

/**
 * The class responsible for communicating with the redis data bus
 *  for queries and responses to and from the database.
 */
class RedisDataBus final {
private:

	/**
	 * Redis Data Context
	 */
	RedisDataCtx _RDCtx;

	/**
	 * RedisAsyncConnect to listening for queries from the bus.
	 */
	redisAsyncContext *_c_in;

	/**
	 * RedisAsyncConnect to sending responses to the redis bus.
	 */
	redisAsyncContext *_c_out;

	/**
	 * The queue of responses from the database ready to be sent to the redis bus.
	 */
	std::queue<std::string> _publishData;

	//DataQueueStatus _dataQueueStatus;

public:
	RedisDataBus();
	virtual ~RedisDataBus();

	/**
	 * Setting variables in context for redis data bus.
	 * Setting timers for reconnect and sending responses to the data bus.
	 * Connecting to the database query bus.
	 */
	void setContext( Context *inCtx );

	/**
	 * Adding a response to the queue from which information is sent to the data bus.
	 *
	 * @param inData Frame in json format, compliant with the protocol.
     *               Response to a database query.
	 */
	void addToSend( const std::string inData );

	/**
	 * Disconnecting from the database query bus.
	 */
	void disconnect();

	/**
	 * Connecting to the database query bus.
	 */
	void reconnect();

	/**
	 * Terminating the process of reconnecting the query bus to the database.
	 */
	void stopRedisReconnect();

	/**
	 * Beginning the process of reconnecting to the query bus to the database.
	 */
	void startRedisReconnect();

	/**
	 * Starts the timer necessary to transfer data to the bus.
	 * Sets the appropriate db_worker status.
	 */
	void startRedisToSend();

	/**
	 * Setting the db_worker status to OPEN.
     * Sending status to the management bus.
	 */
	void setQueueStatus_OPEN();

	/**
	 * Setting the db_worker status to CLOSED.
     * Sending status to the management bus.
	 */
	void setQueueStatus_CLOSED();

	/**
	 * Setting the db_worker status to START.
     * Sending status to the management bus.
	 */
	void setQueueStatus_START();

	/**
	 * Setting the db_worker status to BLOCKED.
     * Sending status to the management bus.
	 */
	void setQueueStatus_BLOCKED();

	/**
	 * Setting the db_worker status to STAND_BY.
     * Sending status to the management bus.
	 */
	void setQueueStatus_STAND_BY();

	/**
	 * Passing the response from the database to the response queue.
	 *
	 * @param inDataDBResponse The response from the database.
	 */
	void sendDBDataResponse(DataDBResponse* inDataDBResponse );

	/**
	 * Checking whether the current bd_worker status is the same as the one given as a parameter.
	 *
	 * @param inDataQueueStatus Status to compare with the db_worker status.
	 *
	 * @return True if the db_worker status is identical to the parameter.
	 */
	bool isQueueStatus( QueueStatus::Type inDataQueueStatus );

	/**
	 * Getting db_worker status.
	 *
	 * @return db_worker status.
	 */
	QueueStatus::Type getQueueStatus();

private:
   /**
    * Connecting to the database query bus.
    */
	void _connectToRedis();

	/**
	 * Connection to the redis communication bus, receiving messages.
	 *
	 * @param inIP The IP address of the redis server.
	 * @param inPort The port on which the redis service listens.
	 */
	void _connectInRedis(const std::string &inIP, const int inPort);

	/**
	 * Connecting to the Redis communication bus, sending messages.
	 *
	 * @param inIP The IP address of the redis server.
	 * @param inPort The port on which the redis service listens.
	 */
	void _connectOutRedis(const std::string &inIP, const int inPort);

	/**
	 * Connecting communication with redis to libevent.
     * Login to redis.
     * Subscribing to a communication channel.
	 */
	void _attachRedisBase();

	/**
	 * Login to redis.
	 */
	void _authToRedis();

	/**
	 * Subscribing to a communication channel.
	 */
	void _subscribe();

	/**
	 * Enable the timer for sending replies.
	 */
	//void _enableEvtimerToSend();

	/**
	 * Transferring the status of db_worker to the management queue.
	 */
	void _sendQueueStatusToManager();

	/**
	 * Downloading data from the queue.
	 * Forwarding the reply to be sent.
	 *
	 * @param inData Frame in json format, compliant with the protocol.
     *               Response to a database query.
	 */
	void _sendData(const std::string inData);

	/**
	 * Publishing data to the database query/response bus.
	 *
	 * @param inData Frame in json format, compliant with the protocol.
     *               Response to a database query.
     *
     * @return True if the message was successfully published on the data bus, otherwise false is returned.
	 */
	bool _publish(const std::string inData);

	/**
	 * Callback function for connection to redis.
	 * If an active connection disables the reconnect mechanism,
	 * otherwise it activates the reconnect mechanism.
	 *
	 * @param redisAsyncContext Context for redis connection.
	 * @param status Status provided about connection to redis.
	 */
	static void _appOnConnect(const redisAsyncContext *c, int status);

	/**
	 * Callback function called after disconnecting from redis.
	 *
	 * @param redisAsyncContext Context for redis connection.
	 * @param status Status provided about connection to redis.
	 */
	static void _appOnDisconnect(const redisAsyncContext *c, int status);

	/**
	 * Callback function for the timer necessary for reconnect to redis to work.
	 *
	 * @See libevent callback
	 *
	 * @param fd The file descriptor
	 * @param what Is a set of the flags listed above.
	 * @param ptr The value that was passed in for arg when the function was constructed.
	 */
	static void _RedisReconnect_cb( evutil_socket_t fd, short what, void *ptr );

	/**
	 * Callback triggered after receiving a frame from the data bus.
	 *
	 * @param c Local context for connecting to redis.
	 * @param reply An object containing data received from redis.
	 * @param privdata A pointer pointing to an object representing the handling context of the received frame.
	 */
	static void _onMessage(redisAsyncContext *c, void *reply, void *privdata);
};

#endif /* REDIS_BUS_DATA_REDISDATABUS_H_ */
