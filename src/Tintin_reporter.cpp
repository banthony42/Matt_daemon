// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Tintin_reporter.cpp                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/17 11:58:22 by banthony          #+#    #+#             //
//   Updated: 2019/08/17 11:58:24 by banthony         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Utils.hpp"
#include "Tintin_reporter.hpp"
#include <sstream>
#include <iostream>
#include <ctime>
#include <sys/stat.h>
#include <stdlib.h>

Tintin_reporter *Tintin_reporter::_Instance = NULL;

static const std::string sLogType[LOG_TYPE_COUNT] = {
    [INFO] =    "INFO",
    [WARNING] = "WARNING",
    [ERROR] =   "ERROR",
};

Tintin_reporter *Tintin_reporter::GetInstance() {
	if (!Tintin_reporter::_Instance)
		Tintin_reporter::_Instance = new Tintin_reporter(LOG_FILE_PATH);
	return Tintin_reporter::_Instance;
}

void  Tintin_reporter::DeleteInstance() {
	if (Tintin_reporter::_Instance) {
        delete Tintin_reporter::_Instance;
        Tintin_reporter::_Instance = NULL;
    }
}

/*
**  Default constructor will not create log file, 
**  all messages will be write on STDOUT.
*/
Tintin_reporter::Tintin_reporter()
{
    _FileLogExist = false;
}

Tintin_reporter::Tintin_reporter(Tintin_reporter const &iCopy)
{
    *this = iCopy;
}

Tintin_reporter::~Tintin_reporter()
{
    if (_FileLogExist) {
        try {
            _FileLog.close();
        }
        catch(std::exception const &iException) {
            std::cerr << "[Tintin_reporter]: " << iException.what() << std::endl;
        }
    }
    return ;
}

Tintin_reporter &Tintin_reporter::operator=(Tintin_reporter const &iCopy)
{
    if (this != &iCopy) {
        *this = iCopy;
    }
    return *this;
}

/*
**  Create file log using iPath,
**  all messages will be write in that file.
*/
Tintin_reporter::Tintin_reporter(std::string iPath)
{
    if (!Utils::FileExist(LOG_FILE_DIR)) {
        if(mkdir(LOG_FILE_DIR, 0700) < 0) {
            perror("Matt_daemon directory creation:");
            exit(EXIT_FAILURE);
        }
    }
    try {
        _FileLog.open(iPath.c_str(), std::ofstream::out | std::ofstream::app);
    }
    catch(std::exception const &iException) {
            std::cout << "TEST64\n";
        std::cerr << "[Tintin_reporter]: " << iException.what() << std::endl;
    }
    _FileLogExist = true;
}

/*
**  Write log message with several information:
**  Time using this format: [DD/MM/YYYY-HH:MM:SS]
**  The type of the Log: INFO, WARNING, ERROR.
**  The localization in the code, to know which part, module or feature are sending the message.
*/
void Tintin_reporter::Log(eLogType iLogType, std::string iMessage)
{
    if (iLogType < 0 || iLogType >= LOG_TYPE_COUNT || iMessage.empty())
        return ;

    std::string lLogType = " [" + sLogType[iLogType] + "]";

    std::time_t lTime = std::time(0);   // get time now
    std::tm *lTimeInfo = std::localtime(&lTime);

    char lTimeBuffer[22];
    strftime(lTimeBuffer, 22, "[%d/%m/%Y-%H:%M:%S]", lTimeInfo);

    std::string lOutput;
    lOutput = lTimeBuffer + lLogType + "\t- " + (std::string)EXE_NAME + iMessage; 

    if (_FileLogExist)
        _FileLog << lOutput << std::endl;
    else
        std::cout << lOutput << std::endl;
}