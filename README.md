# Matt_daemon
Daemon basique, qui ouvre un port puis traite et stock les messages reçus.

### To do
* Replace all log by perror when possible during the daemon creation.

* Resolve TODO commented line in code, that cause issue during daemon creation.

* Programm should run only with root access.

* Listen on 4242 and write message in log file.

* Handle quit command on 4242.

* Handle all signal.

* Linux compilation + Test on Linux with real path (/var/lock & /var/log)

* Delete mutex file when an error occur.
