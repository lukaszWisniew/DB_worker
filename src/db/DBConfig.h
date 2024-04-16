/*
 ============================================================================
 Name       :  DBConfig.h
 Date       :  24-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef DB_DBCONFIG_H_
#define DB_DBCONFIG_H_

#include <string>


/**
 * Keep configuration connection to postgres database.
 */
class DBConfig final {
private:
	/**
	 * IP address to database host.
	 */
	std::string _ip;

	/**
	 * Data base name
	 */
	std::string _dbName;

	/**
	 * User name used to connection to the database.
	 */
	std::string _user;

	/**
	 * Password used to connection to database.
	 */
	std::string _pass;

public:
	DBConfig();
	virtual ~DBConfig();

	/**
	 * Set data base name
	 *
	 * @param dbName database name
	 */
	void setDbName(const std::string &inDBName);

	/**
	 * Set IP address to data base
	 *
	 * @param ip IP to host with database
	 */
	void setIp(const std::string &inIP);

	/**
	 * Set password to data base
	 *
	 * @param inPass password to login database
	 */
	void setPass(const std::string &inPass);

	/**
	 * Set user name connection database
	 *
	 * @param inUser user name
	 */
	void setUser(const std::string &inUser);

	/**
	 * Get URL to database
	 *
	 * @return URL used to connect to the database.
	 */
	std::string getURLConnection();

	/**
	 * Show configuration.
	 *
	 * Send on stdout information about configuration.
	 */
	void showConfig();

	/**
	 * Checks the correctness of the set configuration.
	 */
	bool isConfigured() const;
};


#endif /* DB_DBCONFIG_H_ */
