// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Daemon.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/17 15:55:49 by banthony          #+#    #+#             //
//   Updated: 2019/08/17 15:55:50 by banthony         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Daemon.hpp"
#include "Tintin_reporter.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <csignal>

static void SignalHandler(int iSignal);

Daemon::Daemon()
{
    return;   
}

Daemon::Daemon(Daemon &iCopy)
{
    *this = iCopy;
}

Daemon::~Daemon()
{
    return ;
}

Daemon &Daemon::operator=(Daemon &iCopy)
{
    if (this != &iCopy) {
        *this = iCopy;
    }
    return *this;
}

/*
**  Transform the calling process into a Daemon.
*/
void Daemon::Daemonize(std::string iDaemonPath)
{
    // STEP 7 (Check if a daemon is already running)
    if (FileExist(FILE_LOCK_PATH)) {
        Tintin_reporter lConsole = Tintin_reporter();
        lConsole.Log(ERROR, "Trying to create daemon that already running.");
        lConsole.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }

    Tintin_reporter lConsole = Tintin_reporter();

    // STEP 1
    ForkAndKillParent();
    Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);

    // STEP 2
    if (setsid() < 0) {
        lMattDaemonLog.Log(ERROR, "An error occured while creating new session.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }
    // STEP 3
    // TODO - Cause issue
    //CloseAllFileDescriptor();

    // STEP 4
    ForkAndKillParent();
    // STEP 5: All file create by our daemon will have right 700.
    umask(077);
    // Open again our file log (Closed during step 3).

    // TODO - Cause issue
    // lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);

    // STEP 6
    if (chdir(iDaemonPath.c_str()) < 0){
        lMattDaemonLog.Log(ERROR, "An error occured while changing directory.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }

    // STEP 7 (Create a file lock to make sure only one instance is running)
    LockDaemon();
    // STEP 8
    std::signal(SIGTERM, SignalHandler);
}

static void SignalHandler(int iSignal)
{
    Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);

    switch (iSignal)
    {
        case SIGTERM:
        lMattDaemonLog.Log(INFO, "SIGTERM has been caught !");
        if (std::remove(FILE_LOCK_PATH)) {
            perror("Matt_daemon: ");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
        break;
        
        default:
        break;
    }
}

void Daemon::LockDaemon()
{
    int lFd;
    Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);

    if ((lFd = open(FILE_LOCK_PATH, O_RDWR | O_CREAT, 740)) < 0) {
        lMattDaemonLog.Log(ERROR, "Error when creating file lock.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }

    if (flock(lFd, LOCK_EX | LOCK_NB) < 0) {
        lMattDaemonLog.Log(ERROR, "Error while locking the mutex file.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }
}

void Daemon::ForkAndKillParent()
{
    Tintin_reporter lConsole = Tintin_reporter();
    pid_t lChildProcess = 0;

    lChildProcess = fork();

    if (lChildProcess < 0) {
        lConsole.Log(ERROR, "An error occured during the fork.");
        lConsole.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }
    // Kill the parent
    else if (lChildProcess > 0) {
        exit(EXIT_SUCCESS);
    }
}

void Daemon::CloseAllFileDescriptor()
{
    int lNumberOfFileDescriptor = getdtablesize();
    int lFd = -1;

    while (++lFd < lNumberOfFileDescriptor)
        close(lFd);

    // To avoid error with function that read or write on standard I/O
    // We re-open and redirect them to /dev/null

    open("/dev/null", O_RDONLY);    // fd 0 - STDOUT
    open("/dev/null", O_RDWR);      // fd 1 - STDIN
    open("/dev/null", O_RDWR);      // fd 2 - STDERR
}

bool Daemon::FileExist(const std::string &iPath)
{
  struct stat lBuffer;   
  return (stat (iPath.c_str(), &lBuffer) == 0); 
}