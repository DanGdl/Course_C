/*
compile: gcc -Wall -Wextra -Wpedantic ex30_syslog.c -o ex30   OR: gcc ex30_syslog.c -o ex30
run: ./ex30
*/

#include <stdio.h>
#include <errno.h>      // errors
#include <stdlib.h>
#include <unistd.h>     // close
#include <string.h>     // error code to string
#include <syslog.h>

#define LOG_ID "EXAMPLE_SYSLOG"


int main(void) {
    openlog(LOG_ID, LOG_CONS | LOG_PID | LOG_PERROR, LOG_USER);
    syslog(LOG_USER | LOG_INFO, "%s", "Hello world!");
    syslog(LOG_USER | LOG_INFO, "%s %s", "Connection from client", str);

    closelog();
    return EXIT_SUCCESS;
}
