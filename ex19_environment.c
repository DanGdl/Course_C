/*
compile: gcc -Wall -Wextra -Wpedantic ex19_environment.c -o ex19   OR: gcc ex19_environment.c -o ex19
run: ./ex19
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#define LEN_ARRAY(x) (sizeof(x)/sizeof(x[0]))

/*
#define EXIT_SUCCESS // implementation defined
#define EXIT_FAILURE // implementation defined

void exit(int status);  // POSIX
void _Exit(int status); // ISO C

#include <unistd.h>
void _exit(int status);                         // ISO C
void exit(int) __attribute__((__noreturn__));   // GCC, Clang
__declspec(noreturn) extern void exit(int);     // MSVC
*/

#define _GNU_SOURCE
#include <unistd.h>
extern char **environ;

void fnExit1 (void) {
    puts("Exit function 1");
}

void fnExit2(void) {
    puts("Exit function 2");
}


const int limits[] = {
    RLIMIT_AS, RLIMIT_CORE, RLIMIT_CPU, RLIMIT_DATA, RLIMIT_FSIZE,
    RLIMIT_LOCKS, RLIMIT_MEMLOCK, RLIMIT_MSGQUEUE, RLIMIT_NICE,
    RLIMIT_NOFILE, RLIMIT_NPROC, RLIMIT_RSS, RLIMIT_RTPRIO,
    RLIMIT_RTTIME, RLIMIT_SIGPENDING, RLIMIT_STACK
};

int main (void) {
/*
    char* getenv(const char* name);
    int putenv(char* str);              // avoid to use!!!
    int setenv(const char* name, const char* value, int rewrite);
    int unsetenv(const char* name);
*/  
    puts("ENVIRONMENT");
    char** tmp = environ;
    while (*tmp != NULL) {
        puts(*tmp);
        tmp++;
    }

    puts("LIMITS");
    printf("Unlimited value %ld\n", RLIM_INFINITY);
    for (unsigned int i = 0; i < LEN_ARRAY(limits); i++) {
        struct rlimit lim = { 0 }; 
        const int rc = getrlimit(limits[i], &lim);
        if (rc < 0) {
            printf("Failed to get limit on idx %d: %s\n", i, strerror(rc));
        } else {
            printf("Limit on idx %d: current %ld, max %ld\n", i, lim.rlim_cur, lim.rlim_max);
        }
    }
    // int setrlimit(int resource, const struct rlimit* rlptr);


    errno = 0;
    int rc = getpriority(PRIO_PROCESS, 0);
    if (rc < 0 && errno != 0) {
        printf("Failed to get process priority %s\n", strerror(errno));
    } else {
        printf("Process nice value %d\n", rc);
    }
    errno = 0;
    rc = getpriority(PRIO_PGRP, 0);
    if (rc < 0 && errno) {
        printf("Failed to get group priority %s\n", strerror(errno));
    } else {
        printf("Process group nice value %d\n", rc);
    }
    errno = 0;
    rc = getpriority(PRIO_USER, 0);
    if (rc < 0 && errno) {
        printf("Failed to get user priority %s\n", strerror(errno));
    } else {
        printf("Process user nice value %d\n", rc);
    }
    // value in range -20 ... 20
    // int setpriority(int which, id_t who, int value);


    atexit(fnExit1);
    atexit(fnExit2);
    puts("Main function");
    exit(EXIT_SUCCESS);
    // abort(EXIT_FAILURE);
    // _exit(EXIT_FAILURE);

    return EXIT_SUCCESS;
    // return EXIT_FAILURE;
}
