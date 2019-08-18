// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/17 12:29:26 by banthony          #+#    #+#             //
//   Updated: 2019/08/17 15:50:31 by banthony         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

/*
**	DAEMON CREATION STEPS
**
**	1 - fork, the process then kill parent.
**		Orphaned child will become a child of init process, as result
**		our process start operating in brackground.
**
**	2 - setsid, make our process independant.
**		A process receives signals from the terminal that is connected to,
**		each process inherits its parent's controllign tty. Our daemon should not
**		receive signal from the process that started it, so it must detach itself
**		from it's controlling tty.
**		The call of setsid, will place the daemon in a new process group & session,
**		and detech it's controlling terminal.
**
**	3 - close, all file descriptors.
**		File descriptors are inherited to child process when fork is used.
**		Note: Some standard library function may read or write to standard I/O.
**		For safety, these descriptors should be opened and redirected to a harmless I/O
**		such as /dev/null. (Using open / dup).
**		This avoid to get error on routines that read/write on standard I/O,
**		which has been closed before.
**
**  4 - fork again and kill the parent.
**      This make our daemon loose it's leading process status, so we are sure
**      it can't get a tty.
**
**	5 - umask, will protect files that our Daemon will create.
**		This call will restrict file right at creation.
**
**	6 - chdir to a targeted directory.
**		A daemon should run in a known directory, there are many advantages.
**
**	7 - flock, will assure to run only one daemon.
**		At daemon starts, a file must be create using open, and locked using flock.
**		All other instance of our daemon will understand that an instance is already running
**		and this instance should stop.
**
**	8 - signal, will caught signal and call a binded function.
**		A process may receive signal from a user or an other process.
**		Our daemon should have signal handler. Some signal can't be caught. (ex: SIGKILL)
**
**	9 - Log message to a file.
**		Our running daemon creates messages, some are important and should be logged.
**		This project have it's own Log class named Tintin_reporter.
**
**  Sources:
**  http://www.enderunix.org/docs/eng/daemon.php
**  https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux
**  https://stackoverflow.com/questions/881388/what-is-the-reason-for-performing-a-double-fork-when-creating-a-daemon
*/

#include "Tintin_reporter.hpp"
#include "Daemon.hpp"
#include "Server.hpp"
#include <unistd.h>

int main()
{
    // Transform this process into a daemon, running at ./
    Daemon lDaemonizer;
    lDaemonizer.Daemonize("./");

    Tintin_reporter lMattDaemonSys = Tintin_reporter(LOG_FILE_PATH);
    lMattDaemonSys.Log(INFO, "Daemon Started.");

    // Open 4242 with 3 connection max
    Server *DaemonServer = new Server();

    // Main daemon server stay here and fork when receive new client connection.
    while(!DaemonServer->WaitClient())
        ;

    // All connected client continue here
    while(42) {
        if (!DaemonServer->CommandInterpreter())
            break;
    }

    delete DaemonServer;
    lMattDaemonSys.Log(INFO, "Daemon ends with quit command.");
    lDaemonizer.~Daemon();

    return (EXIT_SUCCESS);
}
