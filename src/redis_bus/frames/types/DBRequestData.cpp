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
