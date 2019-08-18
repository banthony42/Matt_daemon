// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Server.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/18 17:37:17 by abara             #+#    #+#             //
//   Updated: 2019/08/18 17:37:19 by abara            ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>

Server::Server()
{
    _ServerLog = Tintin_reporter(LOG_FILE_PATH);
    _sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    struct sockaddr_in lSockIn;
    lSockIn.sin_family = AF_INET;
    lSockIn.sin_port = htons(SERVER_PORT);
    lSockIn.sin_addr.s_addr = htonl(INADDR_ANY);
        _ServerLog.Log(WARNING, "TEST");
    if ((bind(_sock, (const struct sockaddr*)&lSockIn, sizeof(lSockIn))) < 0) {
        _ServerLog.Log(ERROR, "Error during binding socket.");
        _ServerLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }

    listen(_sock, MAX_CONNECT);
    return;   
}

Server::Server(Server &iCopy)
{
    *this = iCopy;
}

Server::~Server()
{
    if (close(_sock) < 0) {
        _ServerLog.Log(ERROR, "Error while closing socket.");
    }
    _ServerLog.~Tintin_reporter();
    return ;
}

Server &Server::operator=(Server &iCopy)
{
    if (this != &iCopy) {
        *this = iCopy;
    }
    return *this;
}

bool Server::WaitClient()
{
    struct sockaddr_in  lClientSockIn;
    unsigned int        lClientSockInLen;

    if ((_client_sock = accept(_sock, (struct sockaddr*)&lClientSockIn, &lClientSockInLen))) {

        if (_client_sock < 0) {
            _ServerLog.Log(ERROR, "Error on client connection.");
            _ServerLog.~Tintin_reporter();
            exit(EXIT_FAILURE);
        }

        pid_t lProcess = 0;
        if ((lProcess = fork()) < 0) {
            _ServerLog.Log(ERROR, "Error when forking server.");
            _ServerLog.~Tintin_reporter();
            exit(EXIT_FAILURE);
        }

        if (lProcess == 0) {
            _ServerLog.Log(INFO, "New connection of :" + std::string(inet_ntoa(lClientSockIn.sin_addr)));
            if (close(_sock) < 0) {
                _ServerLog.Log(ERROR, "Error while closing socket.");
            }
            return true;
        }
    }
    return false;
}

bool Server::CommandInterpreter()
{
    return false;
}