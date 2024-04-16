/*
 ============================================================================
 Name       :  ToFrom_JSON.h
 Date       :  30-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
*/



#ifndef REDIS_BUS_FRAMES_TOFROM_JSON_H_
#define REDIS_BUS_FRAMES_TOFROM_JSON_H_

#include "json.hpp"

#include "types/WorkersStatusData.h"

//---------------------
// BLANC
//---------------------
class DataQueueStatus;
class QueueData;
enum class QueueStatusType;

class DataDBRequest;
class DBRequestData;
class DataDBResponse;
//---------------------

/**
 * Functions used by json.hpp to convert data to json in the opposite direction.
 */

using json = nlohmann::json;

void to_json(json& j, const DataQueueStatus& rdCtx);
void from_json(const json& j, DataQueueStatus& rdCtx);

void to_json(json& j, const QueueData& qd);
void from_json(const json& inJ, QueueData& outQD);

void to_json(json& j, const QueueStatusType& qd);
void from_json(const json& inJ, QueueStatusType& outQST);

void to_json(json& j, const WorkersStatusData& wsd);
void from_json(const json& inJ, WorkersStatusData& outWSD);

void to_json(json& j, const WorkersStatusData::WorkerQueueStatus& wsd);
void from_json(const json& inJ, WorkersStatusData::WorkerQueueStatus& outWQS);


void to_json(json& j, const DataDBRequest& inDataDBRequest);
void from_json(const json& j, DataDBRequest& outDataDBRequest);

void to_json(json& j, const DBRequestData& inDBRequestData);
void from_json(const json& inJ, DBRequestData& outDBRequestData);

void to_json(json& outJ, const DataDBResponse& inDataDBResponse);
void from_json(const json& inJ, DataDBResponse& outDataDBResponse);
//void to_json(json& outJ,std::vector<std::vector<std::string>> &inData);

#endif /* REDIS_BUS_FRAMES_TOFROM_JSON_H_ */
