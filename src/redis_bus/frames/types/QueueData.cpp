/*
 ============================================================================
 Name       :  QueueData.cpp
 Date       :  12-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "QueueData.h"

QueueData::QueueData() {
	clear();
}

QueueData::~QueueData() {}

void
QueueData::clear() {
	dataQueueStatus = QueueStatus::Type::none;
/*	maxSize         = 0;
	openSize        = 0;
	size 			= 0;*/
}
