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
#include <sys/wait.h>
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
    Tintin_reporter *lMattDaemonLog = Tintin_reporter::GetInstance();
    lMattDaemonLog->Log(INFO, "===================== Daemon is starting ... =====================");

    // STEP 7 (Check if a daemon is already running)
    if (Utils::FileExist(FILE_LOCK_PATH)) {
        std::cerr << "Trying to create daemon that already running." << std::endl;
        exit(EXIT_FAILURE);
    }

    // STEP 1
    ForkAndKillParent();

    // STEP 2
    if (setsid() < 0) {
        lMattDaemonLog->Log(ERROR, "An error occured while creating new session.");
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
        lMattDaemonLog->Log(ERROR, "An error occured while changing directory.");
        exit(EXIT_FAILURE);
    }

    // STEP 7 (Create a file lock to make sure only one instance is running)
    LockDaemon();

    // STEP 8
    std::signal(SIGTERM, SignalHandler);
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGQUIT, SignalHandler);
    std::signal(SIGKILL, SignalHandler);
    std::signal(SIGCHLD,SignalHandler);
}

static void SignalHandler(int iSignal)
{
    Tintin_reporter *lMattDaemonLog = Tintin_reporter::GetInstance();

    switch (iSignal)
    {
        case SIGTERM:
        lMattDaemonLog->Log(INFO, "SIGTERM has been caught !");
        Daemon::KillDaemon();
        break;
        
        case SIGINT:
        lMattDaemonLog->Log(INFO, "SIGINT has been caught !");
        Daemon::KillDaemon();
        break;

        case SIGQUIT:
        lMattDaemonLog->Log(INFO, "SIGQUIT has been caught !");
        Daemon::KillDaemon();
        break;

        case SIGKILL:
        lMattDaemonLog->Log(INFO, "SIGKILL has been caught !");
        Daemon::KillDaemon();
        break;

        case SIGCHLD:
        lMattDaemonLog->Log(INFO, "SIGCHLD has been caught ! A client is left !");
        // Wait for any child process
        wait(NULL);
        break;

        default:
        break;
    }
}

void Daemon::KillDaemon()
{
    Tintin_reporter *lMattDaemonLog = Tintin_reporter::GetInstance();
    if (std::remove(FILE_LOCK_PATH)) {
        lMattDaemonLog->Log(ERROR, "Error when deleting file lock.");
        exit(EXIT_FAILURE);
    }
    lMattDaemonLog->Log(INFO, "Daemon stopped.");
    exit(EXIT_SUCCESS);
}

void Daemon::LockDaemon()
{
    int lFd;
    Tintin_reporter *lMattDaemonLog = Tintin_reporter::GetInstance();

    if ((lFd = open(FILE_LOCK_PATH, O_RDWR | O_CREAT, 740)) < 0) {
        lMattDaemonLog->Log(ERROR, "Error when creating file lock.");
        exit(EXIT_FAILURE);
    }

    if (flock(lFd, LOCK_EX | LOCK_NB) < 0) {
        lMattDaemonLog->Log(ERROR, "Error while locking the mutex file.");
        Daemon::KillDaemon();
        exit(EXIT_FAILURE);
    }
}

void Daemon::ForkAndKillParent()
{
    pid_t lChildProcess = 0;

    lChildProcess = fork();

    if (lChildProcess < 0) {
        Tintin_reporter *lMattDaemonLog = Tintin_reporter::GetInstance();
        lMattDaemonLog->Log(ERROR, "An error occured during the fork.");
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

    Tintin_reporter *lMattDaemonLog = Tintin_reporter::GetInstance();

    while (++lFd < lNumberOfFileDescriptor)
        close(lFd);

    // File descriptor has been close so delete old instance and create new one.
    Tintin_reporter::DeleteInstance();
    lMattDaemonLog = Tintin_reporter::GetInstance();

    // To avoid error with function that read or write on standard I/O
    // We re-open and redirect them to /dev/null

    if (open("/dev/null", O_RDONLY) < 0) {    // fd 0 - STDOUT
        lMattDaemonLog->Log(ERROR, "An error occured during STDOUT redirection.");
        exit(EXIT_FAILURE);
    }

    if (open("/dev/null", O_RDWR) < 0) {      // fd 1 - STDIN
        lMattDaemonLog->Log(ERROR, "An error occured during STDIN redirection.");
        exit(EXIT_FAILURE);
        }

    if (open("/dev/null", O_RDWR) < 0) {      // fd 2 - STDERR
        lMattDaemonLog->Log(ERROR, "An error occured during STDERR redirection.");
        exit(EXIT_FAILURE);
        }
}