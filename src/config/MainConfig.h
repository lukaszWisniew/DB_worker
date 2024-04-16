/*
 ============================================================================
 Name       :  MainConfig.h
 Date       :  11-10-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
 */

#ifndef MAINCONFIG_H_
#define MAINCONFIG_H_

/**
 * Class keep main configuration of program.
 */
class MainConfig final {
private:
	/**
	 * A tag holding information about logging on/off.
	 */
	bool _logEnable;
public:
	MainConfig();
	virtual ~MainConfig();

	/**
	 * Get tag describe information about logging on/off.
	 *
	 * @return true if logging is enabled.
	 */
	bool isLogEnable() const;

	/**
	 * Set logging tag.
	 */
	void setLogEnable(bool logEnable);

	/**
	 * Sending settings to standard output.
	 */
	void showConfig();
};

#endif /* MAINCONFIG_H_ */
