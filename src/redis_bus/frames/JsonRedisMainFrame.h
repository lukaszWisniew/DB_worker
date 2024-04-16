/*
 ============================================================================
 Name       :  JsonRedisMainFrame.h
 Date       :  30-11-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef REDIS_BUS_FRAMES_JSONREDISMAINFRAME_H_
#define REDIS_BUS_FRAMES_JSONREDISMAINFRAME_H_

#include <string>

#include "ToFrom_JSON.h"
#include "json.hpp"

using json = nlohmann::json;

/**
 * A class that is inherited by other classes representing upload data received and sent to the redis bus.
 */
class JsonRedisMainFrame {
protected:
  /**
   * json class
   *
   * @see JSON for Modern C++
   *      https://github.com/nlohmann/json
   */
  json j;
public:
  /**
   * Protocol version
   */
  std::string version;

  /**
   * Frame name
   */
  std::string frameType;

protected:
	JsonRedisMainFrame();
public:
	virtual ~JsonRedisMainFrame();

	/**
	 * Virtual function to converting the frame from json to a data structure.
	 *
	 * @param inJSON Frame in json format.
	 * @param outError outError Error content.
	 *
	 * return True if the data can be converted.
	 */
	virtual bool fromJson( const std::string& inJSON, std::string& outError )=0;
};

#endif /* REDIS_BUS_FRAMES_JSONREDISMAINFRAME_H_ */
