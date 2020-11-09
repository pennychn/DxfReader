#pragma once

#define _CRT_SECURE_NO_WARNINGS
/*
 * Logger.h
 *
 *  Created on: Jan 4, 2009
 *      Author: Karl
 */
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#ifndef LOGGER_H_
#define LOGGER_H_

class Logger {
public:
	static Logger* Instance();
	~Logger();

	//Logging Levels
	const static int LOG_LEVEL_NO_LOGGING = 0;
	const static int LOG_LEVEL_FATAL = 1;
	const static int LOG_LEVEL_SEVERE = 2;
	const static int LOG_LEVEL_WARNING = 3;
	const static int LOG_LEVEL_INFO = 4;
	const static int LOG_LEVEL_CONFIG = 5;
	const static int LOG_LEVEL_VERBOSE = 6;
	const static int LOG_LEVEL_VERY_VERBOSE = 7;
	// self-defined by hobo
	const static int LOG_LEVEL_INFO_HEAD = 8;

	//Logging Targets
	const static int LOG_TARGET_FILE = 1;
	const static int LOG_TARGET_STDOUT = 2;
	const static int LOG_TARGET_BOTH = 3;

	void SetLoggingLevel(int);

	void SetLoggingTarget(int);

	void Log(int, std::string );
	void Log(std::string );
	void Log(const char* msg);
	void ErrLog(int , std::string );

	std::ofstream *logFile() { return &info_log; }
	std::ofstream *errFile() { return &err_log; }
// protected:
	Logger();
	Logger(const Logger&);
	Logger& operator= (const Logger&);

private:
	std::ofstream info_log;
	std::ofstream err_log;
	static Logger* pinstance;
};

#endif /* LOGGER_H_ */
