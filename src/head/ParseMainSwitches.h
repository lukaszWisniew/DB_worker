/*
 ============================================================================
 Name       :  ParseMainSwitches.h
 Date       :  29-09-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
*/

#include <config/IParseSwitches.h>
#include <config/IConfiguration.h>
#include <config/ParseMainArgs.h>

/**
 * A class that implements the IParseSwitches interface.
 *
 * The class describes the arguments passed to the program at startup.
 */
class ParseMainSwitches : public lw_lib::IParseSwitches {
private:

	/** Parameters passed to the program at startup that are supported. */
	lw_lib::ParseMainArgs _args;
public:
	ParseMainSwitches();
	virtual ~ParseMainSwitches();

	/**
	 * The method implements the interface of the base class.
	 *
	 * The method recognizes the arguments passed to the program and sets the program configuration accordingly.
	 *
	 * @param argc refers to the number of arguments passed to the program
	 * @param argv is a pointer array which points to each argument passed to the program
	 * @param inOutConfiguration The object contains program configuration variables.
	 */
	virtual void parse(int argc, char **argv, lw_lib::IConfiguration *inOutConfiguration);
private:
	/** Fills the list of supported arguments. */
	virtual void _setParams();
};
