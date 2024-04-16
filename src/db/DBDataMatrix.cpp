/*
 ============================================================================
 Name       :  DBDataMatrix.cpp
 Date       :  25-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "DBDataMatrix.h"

#include <iostream>

DBDataMatrix::DBDataMatrix() {
}

DBDataMatrix::~DBDataMatrix() {
}


void
DBDataMatrix::resize(const int &inRows ) {
	_data.resize(inRows);
}


void
DBDataMatrix::addData(const unsigned int &inRows, const std::string &inData) {
	if (_data.size() < (inRows+1)) {
		_data.resize(inRows+1);
	}
	_data.at(inRows).push_back(inData);
}

void
DBDataMatrix::addError(std::string inError, std::string inErrorDescription) {
	clear();
	resize(2);
	_data.at(0).push_back("error");
	_data.at(0).push_back("description");
	_data.at(1).push_back(inError);
	_data.at(1).push_back(inErrorDescription);

}

std::vector<std::vector<std::string>>&
DBDataMatrix::getData() {
	return _data;
}

void
DBDataMatrix::clear() {
	for (std::vector<std::vector<std::string>>::size_type irow = 0; irow < _data.size(); ++irow) {
		_data[irow].clear();
	}
	_data.clear();
}

void
DBDataMatrix::show() {
	for (std::vector<std::vector<std::string>>::size_type irow = 0; irow < _data.size(); ++irow) {
		for (std::vector<std::string>::size_type icol = 0; icol < _data[irow].size(); ++icol) {
			std::cout << _data[irow][icol] << ", ";
		}
		std::cout << std::endl;
	}
}
