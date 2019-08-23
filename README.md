# Matt_daemon
Daemon basique, qui ouvre un port puis traite et stock les messages reçus.

### To do
* Programm should run only with root access.

* Handle all signal, and find out why some can't be caught by a forked process.


### Bug
* Quitting as a client, using terminal shortcut for signal (SIGINT, SIGQUIT)
  cause the file lock to be deleted.
