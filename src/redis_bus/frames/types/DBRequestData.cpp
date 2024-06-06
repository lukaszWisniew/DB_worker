/*
 ============================================================================
 Name       :  DBRequestData.cpp
 Date       :  22-01-2024
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2024 Lukasz Wisniewski
 ============================================================================
 */

#include "DBRequestData.h"

DBRequestData::DBRequestData() {
	clear();

}

DBRequestData::~DBRequestData() {}

void
DBRequestData::clear() {
	queryName.clear();
	variables.clear();
}


DBRequestData&
DBRequestData::operator=(const DBRequestData& other) {
	if (this == &other) {
		return *this; // samo-przypisanie
	}

	clear();

	queryName = other.queryName;
	variables = other.variables;

	return *this;
}
