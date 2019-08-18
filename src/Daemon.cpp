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

#include "Utils.hpp"
#include "Daemon.hpp"
#include "Tintin_reporter.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
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
    Daemon::KillDaemon();
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
    Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);
    lMattDaemonLog.Log(INFO, "Daemon is starting ...");

    // STEP 7 (Check if a daemon is already running)
    if (Utils::FileExist(FILE_LOCK_PATH)) {
        Tintin_reporter lConsole = Tintin_reporter();
        lConsole.Log(ERROR, "Trying to create daemon that already running.");
        lConsole.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }

    // STEP 1
    ForkAndKillParent();

    // STEP 2
    if (setsid() < 0) {
        lMattDaemonLog.Log(ERROR, "An error occured while creating new session.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }

    // STEP 3
    CloseAllFileDescriptor();

    // STEP 4
    ForkAndKillParent();

    // STEP 5: All file create by our daemon will have right 700.
    umask(077);

    // STEP 6
    if (chdir(iDaemonPath.c_str()) < 0) {
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
        Daemon::KillDaemon();
        break;
        
        default:
        break;
    }
}

void Daemon::KillDaemon()
{
    if (std::remove(FILE_LOCK_PATH)) {
        Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);
        lMattDaemonLog.Log(ERROR, "Error when deleting file lock.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void Daemon::LockDaemon()
{
    int lFd;

    if ((lFd = open(FILE_LOCK_PATH, O_RDWR | O_CREAT, 740)) < 0) {
        Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);
        lMattDaemonLog.Log(ERROR, "Error when creating file lock.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }

    if (flock(lFd, LOCK_EX | LOCK_NB) < 0) {
        Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);
        lMattDaemonLog.Log(ERROR, "Error while locking the mutex file.");
        lMattDaemonLog.~Tintin_reporter();
        Daemon::KillDaemon();
        exit(EXIT_FAILURE);
    }
}

void Daemon::ForkAndKillParent()
{
    pid_t lChildProcess = 0;

    lChildProcess = fork();

    if (lChildProcess < 0) {
        Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);
        lMattDaemonLog.Log(ERROR, "An error occured during the fork.");
        lMattDaemonLog.~Tintin_reporter();
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
    int lFd = 2;

    while (++lFd < lNumberOfFileDescriptor)
        close(lFd);

    Tintin_reporter lMattDaemonLog = Tintin_reporter(LOG_FILE_PATH);
    lMattDaemonLog.Log(INFO, "All file descriptors has been closed.");

    // To avoid error with function that read or write on standard I/O
    // We re-open and redirect them to /dev/null

    if (open("/dev/null", O_RDONLY) < 0) {    // fd 0 - STDOUT
        lMattDaemonLog.Log(ERROR, "An error occured during STDOUT redirection.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
    }

    if (open("/dev/null", O_RDWR) < 0) {      // fd 1 - STDIN
        lMattDaemonLog.Log(ERROR, "An error occured during STDIN redirection.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
        }

    if (open("/dev/null", O_RDWR) < 0) {      // fd 2 - STDERR
        lMattDaemonLog.Log(ERROR, "An error occured during STDERR redirection.");
        lMattDaemonLog.~Tintin_reporter();
        exit(EXIT_FAILURE);
        }

    lMattDaemonLog.~Tintin_reporter();
}