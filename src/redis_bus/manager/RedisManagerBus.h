/*
 ============================================================================
 Name       :  RedisManagerBus.h
 Date       :  02-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_MANAGER_REDISMANAGERBUS_H_
#define REDIS_BUS_MANAGER_REDISMANAGERBUS_H_

#include <queue>
#include <string>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>

#include <log/Log.h>

#include "../../config/Configuration.h"
#include "RedisManagerCtx.h"

//--------------------
// Blank
//--------------------
class Context;
//--------------------

/**
 * The class responsible for communicating with the Redis management bus,
 * Responsible for receiving information about the statuses of other db_workers
 * and sending information about the status of this db_worker.
 */
class RedisManagerBus final {
private:

	/**
	 * Redis Manager Context
	 */
	RedisManagerCtx _RMCtx;

	/**
	 * RedisAsyncConnect to listening for queries from the bus.
	 */
	redisAsyncContext *_c_in;

	/**
	 * RedisAsyncConnect to sending responses to the redis bus.
	 */
	redisAsyncContext *_c_out;

	/**
	 * A queue of frames with db_worker status ready to be sent to the redis management bus.
	 */
	std::queue<std::string> _publishData;

public:
	RedisManagerBus();
	virtual ~RedisManagerBus();

	/**
	 * Setting variables in context for redis management bus.
	 * Setting timers for reconnect and sending responses to the management bus.
	 * Connecting to the management bus.
	 */
	void setContext( Context *inCtx );

	/**
	 * Adding a response to the queue from which information is sent to the management bus.
	 *
	 * @param inData Frame in json format, compliant with the protocol.
	 */
	void addToSend( const std::string inData );

	/**
	 * Disconnecting from the management bus.
	 */
	void disconnect();

	/**
	 * Connecting to the management bus.
	 */
	void reconnect();

	/**
	 * Terminating the process of reconnecting the management bus.
	 */
	void stopRedisReconnect();

	/**
	 * Beginning the process of reconnecting to the management bus.
	 */
	void startRedisReconnect();

private:
   /**
	* Connecting to the management bus.
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
	 * Downloading data from the queue.
	 * Forwarding the reply to be sent.
	 *
	 * @param inData Frame in json format, compliant with the protocol.
	 */
	void _sendData(const std::string inData);

	/**
	 * Publishing data to the database query/response bus.
	 *
	 * @param inData Frame in json format, compliant with the protocol.
     *
     * @return True if the message was successfully published on the management bus, otherwise false is returned.
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
	 * Callback triggered after receiving a frame from the management bus.
	 *
	 * @param c Local context for connecting to redis.
	 * @param reply An object containing data received from redis.
	 * @param privdata A pointer pointing to an object representing the handling context of the received frame.
	 */
	static void _onMessage(redisAsyncContext *c, void *reply, void *privdata);
};

#endif /* REDIS_BUS_MANAGER_REDISMANAGERBUS_H_ */
