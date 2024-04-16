/*
 ============================================================================
 Name       :  DBDispatcher.cpp
 Date       :  24-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "DBDispatcher.h"

#include <iostream>
#include <regex>

DBDispatcher::DBDispatcher() {
	_conf = NULL;
	_log = NULL;
	_dbConnection = NULL;
}

DBDispatcher::~DBDispatcher() {
	_dbConnection->close();
	delete(_dbConnection);
}

void
DBDispatcher::setConfiguration( DBConfig *inDBConfig, lw_lib::Log  *inLog ) {
	_conf = inDBConfig;
	_log = inLog;
	_setConnection();
}

void
DBDispatcher::_setConnection () {
	//_log->sendDebug("Connecting to DB");
	try {
		_dbConnection = new pqxx::connection(_conf->getURLConnection());
	} catch (std::exception const &e) {
		_log->sendError(e.what());
		exit(EXIT_FAILURE);
	}
}

bool
DBDispatcher::query( const std::string &inQueryName, const std::map<std::string, std::string> &inVariableMap, DBDataMatrix *outDBDataMatrix ) const {
	bool output = false;
	pqxx::row_size_type irow = 0;

	std::string fullQuery;

	outDBDataMatrix->clear();


	try {
		pqxx::work txn{*_dbConnection};

		std::string mainQuery = "SELECT query FROM public.queries WHERE name='" + inQueryName + "'";

		//_log->sendDebug("Target question query [" + inQueryName + "].");

		std::string query = txn.query_value<std::string>(mainQuery);

		_setSQLVariables( query, inVariableMap, fullQuery );

		pqxx::result res = txn.exec(fullQuery);

		txn.commit();

		for (pqxx::row_size_type col = 0; col < res.columns(); ++col) outDBDataMatrix->addData(irow, res.column_name(col) );

		for (auto row: res) {
			++irow;
			for (const auto &field: row) outDBDataMatrix->addData(irow, field.c_str());
		}

		output = true;

	} catch (pqxx::sql_error const &e) {
		_log->sendError("DB query[1]: " + std::string(e.what()));
	} catch (std::exception const &e) {
		_log->sendError("DB query[2]: " + std::string(e.what()));
		outDBDataMatrix->addError("r0001", "Query name not found." );
		output = true;
	} catch (const std::string &msg) {
		_log->sendError( "DB query[3]: " + msg );
	}

	return output;
}

bool
DBDispatcher::_setSQLVariables(const std::string &inRawRuery, const std::map<std::string, std::string> &inVariableMap, std::string &outFullQuery) const {
	bool output = true;

	std::regex variableRegex(R"(<([^<>]+)>)");
	std::smatch match;

	outFullQuery = inRawRuery;

	while (std::regex_search(outFullQuery, match, variableRegex)) {
		if (match.size() == 2) {
			std::string variableName = match[1];
			auto it = inVariableMap.find(variableName);
			if (it != inVariableMap.end()) {
				outFullQuery.replace(match.position(), match.length(), it->second);
			} else {
				output = false; // Zmienna nie znaleziona w mapie
				std::string error_info = "Variable [" + variableName + "] not found in the map. Query: \"" + inRawRuery + "\"";
				throw error_info;
				break;
			}
		} else {
			output = false; // Nieprawidłowy format zmiennych w zapytaniu
			std::string error_info = "Invalid format of variables in query. Query: \n" + inRawRuery + "\"";
			throw error_info;
			break;
		}
	}

	return output;
}
