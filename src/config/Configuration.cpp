/*
 ============================================================================
 Name       :  Configuration.cpp
 Date       :  04-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#include "Configuration.h"

#include <iostream>
#include <algorithm>
#include <error.h>


#include <config/ConfigFile.h>
#include <misc/Procedures.h>

#include "../version.h"

Configuration::Configuration() {}

Configuration::~Configuration() {}

ManagerRBConfig*
Configuration::getManagerRBConfig() {
	return &managerRBConfig;
}

DataRBConfig*
Configuration::getDataRBConfig() {
	return &dataRBConfig;
}

void Configuration::readConfigFile() {

	std::vector<std::string> params; //section's parameters
	std::string section = ""; //section value
	std::string param = ""; //parameter name
	std::string value = ""; //parameter value

	lw_lib::ConfigFile configFile(_configFilePath);

	if( configFile.read( ) ){
		/* Read MAIN section */
		section = "MAIN";
		configFile.getSectionParameters(section, params);
		for ( std::vector<std::string>::size_type i = 0; ( i < params.size() ); ++i ) {
			param = params[i];
			std::transform(param.begin(), param.end(),param.begin(), ::toupper);

			if ( param == "LOG_ENABLE" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				mainConfig.setLogEnable(lw_lib::OnOffOnBool(value.c_str()));
			}
		}

		logConf.setVerboseMode( _verbose );

		if ( mainConfig.isLogEnable()) {

			/* Read LOG section */
			section = "LOG";
			configFile.getSectionParameters(section, params);
			for ( std::vector<std::string>::size_type i = 0; ( i < params.size() ); ++i ) {
				param = params[i];
				std::transform(param.begin(), param.end(),param.begin(), ::toupper);

				if ( param == "FILE_PATH" ) {
					value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

					logConf.setFilePath(value);
				} else if ( param == "FILE_NAME" ) {
					value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

					logConf.setFileName(value);
				} else if ( param == "LEVEL" ) {
					value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");
					std::transform(value.begin(), value.end(),value.begin(), ::toupper);

					if ( value == "INFO") {
						logConf.setLoglevel(lw_lib::LogLevel::INFO);
					} else if ( value == "WARNING") {
						logConf.setLoglevel(lw_lib::LogLevel::WARNING);
					} else if ( value == "DEBUG") {
						logConf.setLoglevel(lw_lib::LogLevel::DEBUG);
					}else if ( value == "ERROR") {
						logConf.setLoglevel(lw_lib::LogLevel::ERROR);
					}
				} else if ( param == "ROTATE" ) {
					value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

					logConf.setRotate(lw_lib::OnOffOnBool(value.c_str()));

				} else if ( param == "ADD_PID" ) {
					value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

					logConf.setAddPid(lw_lib::OnOffOnBool(value.c_str()));

				} else if ( param == "ROTATE_TYPE" ) {
					value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");
					std::transform(value.begin(), value.end(),value.begin(), ::toupper);

					if ( value == "SIZE" ) {
						logConf.setRotateFileType(lw_lib::LogRotateType::SIZE);
					} else if (  value == "TIME"  ) {
						logConf.setRotateFileType(lw_lib::LogRotateType::TIME);
					}

				} else if ( param == "ROTATE_SIZE_FILE" ) {
					value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

					logConf.setLogRotateSize(lw_lib::stringTo<uint>(value));

				} else if ( param == "ROTATE_TIME" ) {
					value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

					if ( logConf.checkRotateTimeFormat(value) ) {
						logConf.setLogRotateTime(value);
					} else {}

				}
			}

			if ( (logConf.getFilePath() == "undefined") || (logConf.getFileName() == "undefined") ) {
				error(1, 0, "Undefined path or configuration file name !!");
			}

			if ( logConf.isRotate() ) {
				if (logConf.getLogRotateType() == lw_lib::LogRotateType::SIZE) {
					if (logConf.getLogRotateSize() == 0) {
						error(1, 0, "Set the ROTATE_SIZE_FILE parameter correctly.");
					}
				} else if (logConf.getLogRotateType() == lw_lib::LogRotateType::TIME) {
					if ( logConf.getLogRotateTime() == "undefined" ) {
						error(1, 0, "Set the ROTATE_TIME parameter correctly.");
					}
				} else {
					error(1, 0, "Set the correct rotate parameter.");
				}
			}
		}

		/* Read LOG section */
		section = "DB_CONNECTION";
		configFile.getSectionParameters(section, params);
		for ( std::vector<std::string>::size_type i = 0; ( i < params.size() ); ++i ) {
			param = params[i];
			std::transform(param.begin(), param.end(),param.begin(), ::toupper);

			if ( param == "ADDRESS" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				if (lw_lib::isIpAddress(value.c_str())) {
					dbConfig.setIp(value);
				}
			} else if ( param == "DB_NAME" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				dbConfig.setDbName(value);
			} else if ( param == "USER" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				dbConfig.setUser(value);
			} else if ( param == "PASS" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				dbConfig.setPass(value);
			}
		}

		if ( !dbConfig.isConfigured() ) {
			error(1, 0, "DB configuration is not correctly !!");
		}

		/* Read manager redis bus section configuration*/
		section = "DB_MANAGER_BUS";
		configFile.getSectionParameters(section, params);
		for ( std::vector<std::string>::size_type i = 0; ( i < params.size() ); ++i ) {
			param = params[i];
			std::transform(param.begin(), param.end(),param.begin(), ::toupper);

			if ( param == "ADDRESS" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				if (lw_lib::isIpAddress(value.c_str())) {
					managerRBConfig.setIp(value);
				}
			} else if ( param == "PORT" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");
				int port = lw_lib::stringTo<int>(value);
				if ((port >= 1024) && (port <= 65000)) {
					managerRBConfig.setPort(value);
				}

			} else if ( param == "CHANNEL_NAME" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				managerRBConfig.setChName(value);
			} else if ( param == "PASS" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				managerRBConfig.setPass(value);
			} else if ( param == "STATUS_INTERVAL" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");
				int interval = lw_lib::stringTo<int>(value);
				if (interval > 0 ) {
					managerRBConfig.setStatusInterval(interval);
				}
			}
		}

		if ( !managerRBConfig.isConfigured() ) {
			error(1, 0, "DB_MANAGER_BUS configuration is not correctly !!");
		}

		/* Read data redis bus section configuration*/
		section = "DATA_BUS";
		configFile.getSectionParameters(section, params);
		for ( std::vector<std::string>::size_type i = 0; ( i < params.size() ); ++i ) {
			param = params[i];
			std::transform(param.begin(), param.end(),param.begin(), ::toupper);

			if ( param == "ADDRESS" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				if (lw_lib::isIpAddress(value.c_str())) {
					dataRBConfig.setIp(value);
				}
			} else if ( param == "PORT" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");
				int port = lw_lib::stringTo<int>(value);
				if ((port >= 1024) && (port <= 65000)) {
					dataRBConfig.setPort(value);
				}

			} else if ( param == "CHANNEL_NAME" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				dataRBConfig.setChName(value);
			} else if ( param == "DOMAIN" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				dataRBConfig.setDomain(value);
			} else if ( param == "PASS" ) {
				value = lw_lib::trim(configFile.getParameter( section+"/"+params[i] ), "\" \t\r\n");

				dataRBConfig.setPass(value);
			}
		}

		if ( !dataRBConfig.isConfigured() ) {
			error(1, 0, "DATA_BUS configuration is not correctly !!");
		}

	} else {
		exit(EXIT_FAILURE);
	}

	showConfig();
}

void Configuration::showConfig() {

	if (_verbose) {
		std::cout << "" << std::endl;
		std::cout << PROGRAM_DOC << std::endl;
		std::cout << "" << std::endl;
		std::cout << "[Switches]" << std::endl;
		std::cout << " CONFIG FILE PATH    = " << _configFilePath << std::endl;
		std::cout << " VERBOSE             = " << ( _verbose ? "on" : "off" ) << std::endl;
		std::cout << "[]" << std::endl;
		std::cout << "" << std::endl;

		mainConfig.showConfig();
		if (mainConfig.isLogEnable()) logConf.showConfig();
		dbConfig.showConfig();
		dataRBConfig.showConfig();
		managerRBConfig.showConfig();
	}

}
