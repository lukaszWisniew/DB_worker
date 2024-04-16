/*
 ============================================================================
 Name       :  ParseMainSwitches.cpp
 Date       :  29-09-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
*/

#include "ParseMainSwitches.h"

#include <iostream>
#include <map>

#include "../version.h"

ParseMainSwitches::ParseMainSwitches() {
	_setParams();
}

ParseMainSwitches::~ParseMainSwitches() {}

void
ParseMainSwitches::_setParams() {
	//cout << "ParseMainSwitches::setParams" << endl;

	_args.addArg("h", "-h", "--help",    "Show this help.", false);
	_args.addArg("V", "-V", "--version", "Program version", false);
	_args.addArg("v", "-v", "--verbose", "Verbose mode.", false);
	_args.addArg("c", "-c", "--config", "Path to the configuration file.", true);

	//cout << "ParseMainSwitches::setParams  --- END ---" << endl;
}

void
ParseMainSwitches::parse(int argc, char **argv, lw_lib::IConfiguration *inOutConfiguration) {
	vector<string> arguments(argv + 1, argv + argc);
	map<string, string> testmap;
	string key = "";
	string paramValue;

	for( vector<string>::size_type i_arguments=0; i_arguments < arguments.size(); ++i_arguments) {

		const auto& arg = arguments[i_arguments];

		if (_args.onTheList(arg, key) && key == "V") { //Wersja programu
			_args.printVersion(PROGRAM_DOC, VERSION, GIT_COMMIT, RELEASE_DATE, BUILD_DATE);
			exit(EXIT_SUCCESS);
		} else if (_args.onTheList(arg, key) && key == "h") { //Wyswietlenie pomocy
			_args.printHelp(PROGRAM_DOC);
			exit(EXIT_SUCCESS);
		} else if (_args.onTheList(arg, key) && key == "v") { //Tryb gadatliwy
			inOutConfiguration->_verbose = true;
		} else if (_args.onTheList(arg, key) && key == "c") { //Podanie sciezki do pliku konfiguracyjnego
			paramValue = _getParamValue(arguments, i_arguments);
			if ( paramValue == "" ) {
				cerr << "The path to config file is incorrect. (-c <PATH>, --config <PATH>)" << endl;
				exit(EXIT_FAILURE);
			}
			inOutConfiguration->_configFilePath = paramValue;
		}

	}


	//cout << "End of parseMainSwitches." << endl;
}




