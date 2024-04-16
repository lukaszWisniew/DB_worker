/*
 ============================================================================
 Name       :  QueueStatusType.cpp
 Date       :  20-12-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
*/

#include "QueueStatusType.h"

#include <algorithm>


std::string QueueStatus::typeToString( QueueStatus::Type inQueueStatusType ) {
	std::string outStatus;

	switch (inQueueStatusType) {
		case Type::START:
			outStatus = "start";
			break;
		case Type::BLOCKED:
			outStatus = "blocked";
			break;
		case Type::CLOSED:
			outStatus = "closed";
			break;
		case Type::OPEN:
			outStatus = "open";
			break;
		case Type::STAND_BY:
			outStatus = "stand_by";
			break;
		case Type::none:
		default:
			outStatus = "none";
	}

	return outStatus;
}

QueueStatus::Type QueueStatus::typeFromString( std::string inQueueStatusType ) {
	Type outStatus;

	std::transform(inQueueStatusType.begin(), inQueueStatusType.end(),inQueueStatusType.begin(), ::tolower);
	if (!inQueueStatusType.compare("start")) {
		outStatus = Type::START;
	} else if (!inQueueStatusType.compare("blocked")) {
		outStatus = Type::BLOCKED;
	} else if (!inQueueStatusType.compare("closed")) {
		outStatus = Type::CLOSED;
	} else if (!inQueueStatusType.compare("open")) {
		outStatus = Type::OPEN;
	} else if (!inQueueStatusType.compare("stand_by")) {
		outStatus = Type::STAND_BY;
	} else if (!inQueueStatusType.compare("none")) {
		outStatus = Type::none;
	} else {
		outStatus = Type::none;
	}

	return outStatus;
}

