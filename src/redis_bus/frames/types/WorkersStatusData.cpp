/*
 ============================================================================
 Name       :  WorkersStatusData.cpp
 Date       :  13-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "WorkersStatusData.h"

WorkersStatusData::WorkersStatusData() {
	clear();
}

WorkersStatusData::~WorkersStatusData() {}



void
WorkersStatusData::clear() {
	numberDbWorkers = 0;
	wqs.clear();
}
