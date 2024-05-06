/*
 ============================================================================
 Name       :  ToFrom_JSON.cpp
 Date       :  30-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
*/

#include "ToFrom_JSON.h"

#include <string>
#include <vector>
#include <utility>
#include <map>

#include <misc/Procedures.h>

#include "DataQueueStatus.h"
#include "DataDBRequest.h"
#include "DataDBResponse.h"
#include "../config/QueueStatusType.h"
#include "types/QueueData.h"
#include "types/DBRequestData.h"

using json = nlohmann::json;


//----------------------------------------------------------
//  DataQueueStatus
//----------------------------------------------------------

void
to_json(json& j, const DataQueueStatus& inDqs) {

	std::string queueStatus = QueueStatus::typeToString( inDqs.data.dataQueueStatus );

	j["version"] = inDqs.version;
	j["frameType"] = inDqs.frameType;
	j["pid"] = inDqs.pid;
	j["data"] = json{{"status",queueStatus}/*,{"size",0}, {"maxSize", inDqs.data.maxSize}, {"openSize", inDqs.data.openSize}*/};
}
void
from_json(const json& j, DataQueueStatus& outDqs) {}

//----------------------------------------------------------
//  QueueData
//----------------------------------------------------------

void
to_json(json& j, const QueueData& qd) {}

void
from_json(const json& inJ, QueueData& outQD) {
/*	outQD.maxSize = inJ["maxSize"].get<int>();
	outQD.openSize = inJ["openSize"].get<int>();
	outQD.size = inJ["size"].get<int>();*/
	outQD.dataQueueStatus = inJ["status"].get<QueueStatus::Type>();
}

void to_json(json& j, const QueueStatus::Type& qd) {}
void from_json(const json& inJ, QueueStatus::Type& outQST) {
	outQST = QueueStatus::typeFromString(inJ.get<std::string>());
}

//----------------------------------------------------------
//  WorkersStatusData
//----------------------------------------------------------

void to_json(json& j, const WorkersStatusData& wsd) {}
void from_json(const json& inJ, WorkersStatusData& outWSD) {
	outWSD.numberDbWorkers = inJ["numberDbWorkers"].get<int>();
	outWSD.wqs = inJ["statuses"].get<std::vector<WorkersStatusData::WorkerQueueStatus> >();
}

void to_json(json& j, const WorkersStatusData::WorkerQueueStatus& wsd) {}
void from_json(const json& inJ, WorkersStatusData::WorkerQueueStatus& outWQS) {
	outWQS.pid = inJ["pid"].get<int>();
	outWQS.status = inJ["status"].get<std::string>();
}

//----------------------------------------------------------
//  DataDBRequest
//----------------------------------------------------------

void to_json(json& j, const DataDBRequest& inDataDBRequest) {
	j["data"] = json{{"queryName", inDataDBRequest.data.queryName},json{"variables", inDataDBRequest.data.variables} };
	j["pid"] = inDataDBRequest.pid;
	j["frameType"] = inDataDBRequest.frameType;
	j["version"] = inDataDBRequest.version;
}
void from_json(const json& j, DataDBRequest& outDataDBRequest){}

void to_json(json& j, const DBRequestData& inDBRequestData) {}

void from_json(const json& inJ, DBRequestData& outDBRequestData) {
	if ( inJ.find("queryName") != inJ.end() ) {
		outDBRequestData.queryName = inJ["queryName"].get<std::string>();
	} else {
		outDBRequestData.queryName = "";
	}

	if ( inJ.find("variables") != inJ.end() ) {
		outDBRequestData.variables = inJ["variables"].get<std::map<std::string, std::string> >();
	} else {
		outDBRequestData.variables.clear();
	}
}


void to_json(json& outJ, const DataDBResponse& inDataDBResponse){
	std::vector < std::map<std::string, std::string> > data;
	std::vector <std::string> cols_name;
	std::map<std::string, std::string>  v_row;

	for (auto row = inDataDBResponse.data.begin(); row != inDataDBResponse.data.end(); ++row) {
		v_row.clear();
		for ( std::vector<std::string>::size_type icol = 0; icol < row->size(); ++icol) {



			if ( inDataDBResponse.data.begin() == row ) {
				cols_name.push_back(row->data()[icol]);
			} else {
				v_row[cols_name[icol]] = row->data()[icol];
			}
		}
		if (!v_row.empty()) {
			data.push_back(v_row);
		}
	}

	outJ["pid"] = inDataDBResponse.pid;
	outJ["frameType"] = inDataDBResponse.frameType;
	outJ["version"] = inDataDBResponse.version;
	outJ["data"] = json{{"queryName", inDataDBResponse.queryName},json{"responseData", data } };
}
void from_json(const json& inJ, DataDBResponse& outDataDBResponse) {}

