#include "Logger.h"

/*
 * Logger.cpp
 *
 *  Created on: Jan 4, 2009
 *      Author: Karl
 */

#include "Logger.h"

Logger* Logger::pinstance = 0;
int loggingLevel = 8;
int loggingTarget = 1;

Logger* Logger::Instance()
{
	if (pinstance == 0) {
		pinstance = new Logger;
	}
	return pinstance;
}

Logger::Logger()
{
	info_log.open ("logger.log");
	err_log.open ("error.log");
	time_t rawtime;
	char dateName[20];
	dateName[0] = '\0';
	rawtime = time(NULL);
	if (rawtime != -1)
		strftime(dateName, 20, "%d-%m-%Y", gmtime(&rawtime) );

	Log(LOG_LEVEL_INFO, "Log Opened");
	ErrLog(LOG_LEVEL_INFO, "Log Opened");
}

Logger::~Logger()
{
	info_log.close();
	err_log.close();
}

void Logger::SetLoggingLevel(int logLevel)
{
	if ((logLevel >= LOG_LEVEL_NO_LOGGING) && (logLevel <= LOG_LEVEL_INFO_HEAD))
	{
		loggingLevel = logLevel;
	}
}

void Logger::SetLoggingTarget(int logTarget)
{
	if ((logTarget >= LOG_TARGET_FILE) && (logTarget <= LOG_TARGET_BOTH))
	{
		loggingTarget = logTarget;
	}
}

void Logger::Log(int logLevel, std::string logMsg)
{
	if (logLevel <= loggingLevel)
	{
		std::string msgToWrite;
		time_t rawtime;
		struct tm * timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		msgToWrite += asctime(timeinfo);
		msgToWrite.erase(msgToWrite.end()-1,msgToWrite.end());
		msgToWrite += " - ";

		switch (logLevel) {
			case LOG_LEVEL_FATAL:
				msgToWrite += "FATAL - ";
				break;
			case LOG_LEVEL_SEVERE:
				msgToWrite += "SEVERE - ";
				break;
			case LOG_LEVEL_WARNING:
				msgToWrite += "WARNING - ";
				break;
			case LOG_LEVEL_INFO:
				msgToWrite += "INFO - ";
				break;
			case LOG_LEVEL_CONFIG:
				msgToWrite += "CONFIG - ";
				break;
			case LOG_LEVEL_VERBOSE:
				msgToWrite += "VERBOSE - ";
				break;
			case LOG_LEVEL_VERY_VERBOSE:
				msgToWrite += "VERY VERBOSE - ";
				break;
			case LOG_LEVEL_INFO_HEAD:
				msgToWrite += "=============\n"; 
				break;
			default:
				msgToWrite += "UNKNOWN LEVEL - ";
				break;
		}

		if (logLevel == LOG_LEVEL_INFO_HEAD)
		{
			//msgToWrite += "=================> "; 
			msgToWrite += logMsg;
			msgToWrite += "\n";
			//msgToWrite += "<=================\n";
			msgToWrite += "======================";
		}
		else
			msgToWrite += logMsg;

		msgToWrite += "\n";


		switch(loggingTarget){
			case LOG_TARGET_FILE:
				info_log << msgToWrite;
				break;
			case LOG_TARGET_STDOUT:
				std::cout << msgToWrite;
				break;
			case LOG_TARGET_BOTH:
				msgToWrite += "\n";
				info_log << msgToWrite;
				std::cout << msgToWrite;
				break;
			}
	info_log.flush();
	}
}

void Logger::Log(std::string logMsg)
{
	std::string msgToWrite;
	//time_t rawtime;
	//struct tm * timeinfo;

	//time ( &rawtime );
	//timeinfo = localtime ( &rawtime );
	//msgToWrite += asctime(timeinfo);
	//msgToWrite.erase(msgToWrite.end()-1,msgToWrite.end());
	//msgToWrite += " - ";
	//msgToWrite += "INFO - ";
	//msgToWrite += logMsg;
	//msgToWrite += "\n";
	msgToWrite += "=======================\n"; 
	msgToWrite += logMsg;
	msgToWrite += "\n=======================\n"; 

	switch(loggingTarget)
	{
		case LOG_TARGET_FILE:
			info_log << msgToWrite;
			break;
		case LOG_TARGET_STDOUT:
			std::cout << msgToWrite;
			break;
		case LOG_TARGET_BOTH:
			msgToWrite += "\n";
			info_log << msgToWrite;
			std::cout << msgToWrite;
			break;
	}

	info_log.flush();
}

void Logger::Log(const char* msg)
{
	std::string logMsg(msg);
	std::string msgToWrite;
	//time_t rawtime;
	//struct tm * timeinfo;

	//time ( &rawtime );
	//timeinfo = localtime ( &rawtime );
	//msgToWrite += asctime(timeinfo);
	//msgToWrite.erase(msgToWrite.end()-1,msgToWrite.end());
	//msgToWrite += " - ";
	//msgToWrite += "INFO - ";
	//msgToWrite += logMsg;
	//msgToWrite += "\n";
	msgToWrite += "=======================\n"; 
	msgToWrite += logMsg;
	msgToWrite += "\n=======================\n"; 


	switch(loggingTarget)
	{
		case LOG_TARGET_FILE:
			info_log << msgToWrite;
			break;
		case LOG_TARGET_STDOUT:
			std::cout << msgToWrite;
			break;
		case LOG_TARGET_BOTH:
			msgToWrite += "\n";
			info_log << msgToWrite;
			std::cout << msgToWrite;
			break;
	}

	info_log.flush();
}

void Logger::ErrLog(int logLevel, std::string logMsg)
{
	if (logLevel <= loggingLevel)
	{
		std::string msgToWrite;
		time_t rawtime;
		struct tm * timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		msgToWrite += asctime(timeinfo);
		msgToWrite.erase(msgToWrite.end()-1,msgToWrite.end());
		msgToWrite += " - ";

		switch (logLevel) {
			case LOG_LEVEL_FATAL:
				msgToWrite += "FATAL - ";
				break;
			case LOG_LEVEL_SEVERE:
				msgToWrite += "SEVERE - ";
				break;
			case LOG_LEVEL_WARNING:
				msgToWrite += "WARNING - ";
				break;
			case LOG_LEVEL_INFO:
				msgToWrite += "INFO - ";
				break;
			case LOG_LEVEL_CONFIG:
				msgToWrite += "CONFIG - ";
				break;
			case LOG_LEVEL_VERBOSE:
				msgToWrite += "VERBOSE - ";
				break;
			case LOG_LEVEL_VERY_VERBOSE:
				msgToWrite += "VERY VERBOSE - ";
				break;
			case LOG_LEVEL_INFO_HEAD:
				msgToWrite += "==========HEAD LEVEL=========\n"; 
				break;
			default:
				msgToWrite += "UNKNOWN LEVEL - ";
				break;
		}

		msgToWrite += logMsg;
		msgToWrite += "\n";

		err_log << msgToWrite;
		err_log.flush();
	}
}

