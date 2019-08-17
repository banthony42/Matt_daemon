// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Tintin_reporter.hpp                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/17 12:03:14 by banthony          #+#    #+#             //
//   Updated: 2019/08/17 12:03:15 by banthony         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TINTIN_REPORTER_HPP
#define TINTIN_REPORTER_HPP

#include <string>
#include <iostream>
#include <fstream>

#define EXE_NAME "Matt_daemon: "

enum eLogType {
    INFO,
    WARNING,
    ERROR,
    LOG_TYPE_COUNT,
};

class Tintin_reporter {

    public:

    Tintin_reporter(std::string iPath);

    Tintin_reporter();                                          // Canonical
    Tintin_reporter(Tintin_reporter const &iCopy);              // Canonical
    ~Tintin_reporter();                                         // Canonical
    Tintin_reporter &operator=(Tintin_reporter const &iCopy);   // Canonical

    void Log(eLogType iLogType, std::string iMessage);

    private:

    bool            _FileLogExist;
    std::ofstream   _FileLog;
};

#endif