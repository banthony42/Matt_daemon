// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Daemon.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/17 15:55:43 by banthony          #+#    #+#             //
//   Updated: 2019/08/17 15:55:44 by banthony         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef DAEMON_HPP
#define DAEMON_HPP

#include <stdlib.h>

#if __linux__

#define FILE_LOCK_PATH "/var/lock/matt_daemon.lock"

#elif __APPLE__

#define FILE_LOCK_PATH "./matt_daemon.lock"

#endif

class Daemon
{
    public:

    Daemon();                           // Coplien, Canonical
    Daemon(Daemon &iCopy);              // Coplien, Canonical
    ~Daemon();                          // Coplien, Canonical
    Daemon &operator=(Daemon &iCopy);   // Coplien, Canonical

    void Daemonize(std::string iDaemonPath);
    static void KillDaemon();

    private:

    void ForkAndKillParent();
    void CloseAllFileDescriptor();
    void LockDaemon();
};

#endif