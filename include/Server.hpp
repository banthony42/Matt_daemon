// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Server.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/18 17:37:28 by abara             #+#    #+#             //
//   Updated: 2019/08/18 17:37:29 by abara            ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Tintin_reporter.hpp"

#define SERVER_PORT 4242

#define MAX_CONNECT 3

class Server
{
    public:

    Server();                           // Coplien, Canonical
    Server(Server &iCopy);              // Coplien, Canonical
    ~Server();
    Server &operator=(Server &iCopy);   // Coplien, Canonical

    bool WaitClient();
    bool CommandInterpreter();
    
    private:

    int             _sock;
    int             _client_sock;
    Tintin_reporter _ServerLog;
};