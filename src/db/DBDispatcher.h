/*
 ============================================================================
 Name       :  DBDispatcher.h
 Date       :  24-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef DB_DBDISPATCHER_H_
#define DB_DBDISPATCHER_H_

#include <pqxx/pqxx>

#include <map>
#include <string>
#include <log/Log.h>

#include "DBConfig.h"
#include "DBDataMatrix.h"
/**
 * Class responsible for communication with the postgresql database.
 */
class DBDispatcher final {
private:
	/**
	 * Configuration
	 */
	DBConfig *_conf;

	/**
	 * log engine
	 */
	lw_lib::Log  *_log;

	/**
	 * Connection to db.
	 */
	pqxx::connection *_dbConnection;
public:
	DBDispatcher();
	virtual ~DBDispatcher();

	/**
	 * Setting the database connection configuration.
	 *
	 * @param inDBConfig configuration db
	 * @param inLog Passing the logging engine.
	 */
	void setConfiguration( DBConfig *inDBConfig, lw_lib::Log  *inLog );

	/**
	 * Sending a query to the database.
	 *
	 * @param queryName query name
	 * @param inVariableMap Map of variables passed to the query.
	 * @param[out] outDBDataMatrix A matrix containing the answer from the query. Provided the query executed correctly.
	 *
	 * @return Query execution marker. True if the query completed successfully.
	 */
	bool query( const std::string &inQueryName, const std::map<std::string, std::string> &inVariableMap, DBDataMatrix *outDBDataMatrix ) const;

private:
	/**
	 * Creates a connection to the database.
	 */
	void _setConnection ();

	/**
	 * Replaces variables in the query with the provided parameters.
	 *
	 * @param inRawRuery The raw query containing the variables to be replaced.
	 * @param inVariableMap Parameter map with data to be replaced.
	 * @param outFullQuery Query with replaced variables.
	 *
	 * @return True if you managed to replace variables in the raw query.
	 */
	bool _setSQLVariables(const std::string &inRawRuery, const std::map<std::string, std::string>& inVariableMap, std::string &outFullQuery) const;
};

#endif /* DB_DBDISPATCHER_H_ */
