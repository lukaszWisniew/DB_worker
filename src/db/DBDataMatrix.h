/*
 ============================================================================
 Name       :  DBDataMatrix.h
 Date       :  25-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef DB_DBDATAMATRIX_H_
#define DB_DBDATAMATRIX_H_

#include <vector>
#include <string>

/**
 * A class used to pass data resulting from a query to the postgres database.
 */
class DBDataMatrix final {
private:

	/**
	 * A container containing data.
	 */
	std::vector<std::vector<std::string>> _data;
public:
	DBDataMatrix();
	virtual ~DBDataMatrix();

	/**
	 * Changing the container size.
	 *
	 * @param inRows Number of rows
	 */
	void resize(const int &inRows );

	/**
	 * Adding data to the container.
	 *
	 * @param inRows Row number
	 * @param inData data
	 */
	void addData(const unsigned int &inRows, const std::string &inData);

	/**
	 * Adding an error instead of a response from the database.
	 * @param inError error number
	 * @param inErrorDescription Description of the error
	 */
	void addError(std::string inError, std::string inErrorDescription);


	std::vector<std::vector<std::string>>& getData();

	/**
	 * Clearing the container of data and shrinking it to 0,0.
	 */
	void clear();

	/**
	 * View data from a container.
	 * Send data to standard output.
	 */
	void show();

};

#endif /* DB_DBDATAMATRIX_H_ */
