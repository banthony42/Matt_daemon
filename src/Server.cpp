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
#include <cstring>
#include <cerrno>

Server::Server()
{
    _ServerLog = Tintin_reporter::GetInstance();

    _IsServerSide = true;
    _sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in lSockIn;
    lSockIn.sin_family = AF_INET;
    lSockIn.sin_port = htons(SERVER_PORT);
    lSockIn.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(_sock, (const struct sockaddr*)&lSockIn, sizeof(lSockIn))) < 0)
        throw Server::ServerException("Error while starting the server.");

    if (listen(_sock, MAX_CONNECT) < 0)
        _ServerLog->Log(WARNING, "Connection limtation has failed.");

    _ServerLog->Log(INFO, "Server started.");
    return;   
}

Server::Server(Server &iCopy)
{
    *this = iCopy;
}

Server::~Server()
{
    if (_IsServerSide) {
        if (close(_sock) < 0)
            _ServerLog->Log(ERROR, "Error when closing socket: " + std::string(std::strerror(errno)));
    }
    else {
        if (close(_client_sock) < 0)
            _ServerLog->Log(ERROR, "Error when closing socket: " + std::string(std::strerror(errno)));
    }

    _ServerLog->Log(INFO, "Server stopped.");
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

        if (_client_sock < 0)
            throw Server::ServerException("Error while connecting a client.");

        pid_t lProcess = 0;
        if ((lProcess = fork()) < 0)
            throw Server::ServerException("Error while instanciating client space.");

        if (lProcess == 0) {
            _ServerLog->Log(INFO, "New client connection :" + std::string(inet_ntoa(lClientSockIn.sin_addr)));
            if (close(_sock) < 0)
                _ServerLog->Log(ERROR, "Error when closing socket: " + std::string(std::strerror(errno)));
            _IsServerSide = false;
            return true;
        }
    }
    return false;
}

bool Server::ClientCommandInterpreter()
{
    char lRecvBuffer[1024] = {'\0'};
    size_t lRecvData = 0;

    if ((lRecvData = recv(_client_sock, lRecvBuffer, 1024 - 1, 0)) <= 0) {
        _ServerLog->Log(WARNING, "recv end with an error, or client is left.");
        return false;
    }

    std::string lClientInput = std::string(lRecvBuffer, lRecvData);
    lClientInput[lClientInput.find_first_of('\n')] = '\0';

    if (lClientInput == "quit") {
        _ServerLog->Log(INFO, "Client request to quit.");
        return false;
    }

    _ServerLog->Log(INFO, "[Client:]" + lClientInput);
    return true;
}

bool Server::IsServerSide()
{
    return _IsServerSide;
}

Server::ServerException::ServerException(std::string iMessage) : _errorMessage(iMessage) {
}

const char *Server::ServerException::what() const throw() {
	return this->_errorMessage.c_str();
}

Server::ServerException::ServerException() : _errorMessage("Server error:") {
}

Server::ServerException::ServerException(const Server::ServerException &iCopy) {
	if (this != &iCopy)
		*this = iCopy;
}

Server::ServerException::~ServerException() throw() {
}

Server::ServerException Server::ServerException::operator=(const Server::ServerException &iCopy) {
	return iCopy;
}
