# linux-daemon
A daemon that prints information both to a system log (and possibly a log file in /tmp). The daemon writes to the log upon receiving an ALARM signal, and exit upon receiving SIGHUP signal.
