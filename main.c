#include <stdio.h> // perror, printf
#include <sys/types.h> // pid_t
#include <unistd.h> // sleep
// use the extended signal handler instead of void (*X)(int)
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <syslog.h>
#define LOG_DAEMON_NAME "OMA DEMONI"
#define LOG_FACILITY LOG_DAEMON
#define LOG_PRIORITY LOG_ERR

void daemonize(void) {

pid_t pid;
pid = fork();

if (pid < 0) {
    perror("on fork while daemonizing");
    exit(EXIT_FAILURE);
}

if (pid > 0) {
// if we're the parent, we will terminate now
// so that child will be moved under init
exit(EXIT_SUCCESS);
}

// create a new session and put us into it

if (setsid() < 0) {
perror("on session creation while daemonizing");
exit(EXIT_FAILURE);
}

if (chdir("/") < 0) {
perror("on root dir change while daemonizing");
exit(EXIT_FAILURE);
}

// reset umask
umask(0);

// we don't need the normal fds anymore
// we use the symbolic constants here so that it's clear
// what w're doing

close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);

}

void myH(int signum) {

    syslog(LOG_PRIORITY, "A signal came in %d\n", signum);

    if (signum == SIGALRM)
        syslog(LOG_PRIORITY, "SIG: ALARM signal number %d\n", signum);
    else if (signum == SIGHUP)
    {
        syslog(LOG_PRIORITY, "HUP signal %d\n", signum );
        exit(EXIT_SUCCESS);
    }

    }


int main(int argc, char** argv) {

pid_t pid, ppid;
int i = 0;
struct sigaction sa;


// open logging facility
// we want the system logger (syslogd) to add the
// PID to each message, hence the 'LOG_PID' flag

openlog(LOG_DAEMON_NAME, LOG_PID, LOG_FACILITY);
syslog(LOG_PRIORITY,"main[pid=%d,ppid=%d]: going daemon\n", pid, ppid);

daemonize();

// we cannot use printf anymore. stdout is closed
// nothing will be printed here (infact, printf
// returns error here, but won't check that)

sa.sa_handler = myH;
// don't block additional signals while handling this one
sigemptyset(&sa.sa_mask);
// set single action and extended signal handler
sa.sa_flags = 0 ; //SA_ONESHOT | SA_SIGINFO;

    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        perror("Problem installing SIGALRM-handler");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGHUP, &sa, NULL) == -1)
    {
        perror("Problem installing SIGALRM-handler");
        exit(EXIT_FAILURE);
    }

pid = getpid();
ppid = getppid();
// refresh the p[p]ids

pid = getpid();
ppid = getppid();

syslog(LOG_PRIORITY, "main[pid=%d,ppid=%d]: am daemon, starting loop\n",pid, ppid);

while(i < 100) {

syslog(LOG_PRIORITY,"iteration %.2d, waiting for 1s\n", i++);
sleep(1);
}

syslog(LOG_PRIORITY, "stopping\n");

return EXIT_SUCCESS;

}





