/*
 ============================================================================
 Name       :  DBRequestData.h
 Date       :  22-01-2024
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2024 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_FRAMES_TYPES_DBREQUESTDATA_H_
#define REDIS_BUS_FRAMES_TYPES_DBREQUESTDATA_H_

#include <string>
#include <map>

/**
 * Query data from the redis bus.
 * I describe the name of the query and the variables necessary to execute it.
 */
class DBRequestData final {
public:
	/**
	 * Query name.
	 */
	std::string queryName;

	/**
	 * Variables and variable values passed to the query.
	 */
	std::map<std::string, std::string> variables;
public:
	DBRequestData();
	virtual ~DBRequestData();

	/**
	 * Clearing variables.
	 */
	void clear();
};

#endif /* REDIS_BUS_FRAMES_TYPES_DBREQUESTDATA_H_ */
