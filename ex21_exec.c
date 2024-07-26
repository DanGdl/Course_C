/*
compile: gcc -Wall -Wextra -Wpedantic ex21_exec.c -o ex21   OR: gcc ex21_exec.c -o ex21
run: ./ex21
*/

#include <stdio.h>
#include <errno.h>  // errno
#include <string.h> // strerror
#include <stdlib.h>
#include <unistd.h> // exec

int main(void) {
        const char* env_args[] = { 
        "PATH=/usr/bin",
        "HOME=/home/username",
        NULL // last element must be NULL
    }; 
    const char* args[] = { 
        NULL, // here must be a name of program, we'll replace it late
        "-l", "-h",
         NULL // last element must be NULL
    };
    int rc = 0;
    
#if 0
#endif

    // execute list (first 2 parameters must be same!)
    rc = execl("/usr/bin/ls", "/usr/bin/ls", "-l", "-h", NULL);
    if (rc < 0) {
        printf("Failed to execute command: %s\n", strerror(errno));
    }

    // execute list (first 2 parameters must be same!) + search in PATH
    rc = execlp("ls", "ls", "-l", "-h", NULL);
    if (rc < 0) {
        printf("Failed to execute command: %s\n", strerror(errno));
    }

    // execute list (first 2 parameters must be same!) + provide environment variables
    rc = execle("/usr/bin/ls", "/usr/bin/ls", "-l", "-h",  NULL, (char**) env_args);
    if (rc < 0) {
        printf("Failed to execute command: %s\n", strerror(errno));
    }

    // execute vector
    args[0] = "/usr/bin/ls";
    rc = execv("/usr/bin/ls", (char**) args);
    if (rc < 0) {
        printf("Failed to execute command: %s\n", strerror(errno));
    }

    // execute vector + search in PATH
    args[0] = "ls";
    rc = execvp("ls", (char**) args);
    if (rc < 0) {
        printf("Failed to execute command: %s\n", strerror(errno));
    }

    // execute vector (first 2 parameters must be same!) + provide environment variables
    args[0] = "/usr/bin/ls";
    rc = execve("/usr/bin/ls", (char**) args, (char**) env_args);
    if (rc < 0) {
        printf("Failed to execute command: %s\n", strerror(errno));
    }
    return 0;
}
