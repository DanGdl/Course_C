/*
compile: gcc -Wall -Wextra -Wpedantic ex22_fork.c -o ex22   OR: gcc ex22_fork.c -o ex22
run: ./ex22
*/

#include <stdio.h>
#include <errno.h>      // errno
#include <string.h>     // strerror
#include <stdlib.h>
#include <unistd.h>     // exec
#include <sys/wait.h>   // waitpid


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

    int p_rc = 0;
    pid_t pid = fork();
    if (pid < 0) {
        printf("Failed to fork: %s\n", strerror(errno));
    } 
    else if (pid == 0) {
        puts("execl");
        // execute list (first 2 parameters must be same!)
        const int rc = execl("/usr/bin/ls", "/usr/bin/ls", "-l", "-h", NULL);
        if (rc < 0) {
            printf("Failed to execute command: %s\n", strerror(errno));
        }
        return 0;
    }
    // https://linux.die.net/man/2/waitpid
    if (waitpid(pid, &p_rc, 0) < 0) {
        printf("Failed to wait for pid %d: %s\n", pid, strerror(errno));
    }


    pid = fork();
    if (pid < 0) {
        printf("Failed to fork: %s\n", strerror(errno));
    } 
    else if (pid == 0) {
        puts("execlp");
        // execute list (first 2 parameters must be same!) + search in PATH
        const int rc = execlp("ls", "ls", "-l", "-h", NULL);
        if (rc < 0) {
            printf("Failed to execute command: %s\n", strerror(errno));
        }
        return 0;
    }
    if (waitpid(pid, &p_rc, 0) < 0) {
        printf("Failed to wait for pid %d: %s\n", pid, strerror(errno));
    }
    

    pid = fork();
    if (pid < 0) {
        printf("Failed to fork: %s\n", strerror(errno));
    } 
    else if (pid == 0) {
        puts("execle");
        // execute list (first 2 parameters must be same!) + provide environment variables
        const int rc = execle("/usr/bin/ls", "/usr/bin/ls", "-l", "-h", NULL, (char**) env_args);
        if (rc < 0) {
            printf("Failed to execute command: %s\n", strerror(errno));
        }
        return 0;
    }
    if (waitpid(pid, &p_rc, 0) < 0) {
        printf("Failed to wait for pid %d: %s\n", pid, strerror(errno));
    }
    

    pid = fork();
    if (pid < 0) {
        printf("Failed to fork: %s\n", strerror(errno));
    } 
    else if (pid == 0) {
        puts("execv");
        // execute vector
        args[0] = "/usr/bin/ls";
        const int rc = execv("/usr/bin/ls", (char**) args);
        if (rc < 0) {
            printf("Failed to execute command: %s\n", strerror(errno));
        }
        return 0;
    }
    if (waitpid(pid, &p_rc, 0) < 0) {
        printf("Failed to wait for pid %d: %s\n", pid, strerror(errno));
    }
    

    pid = fork();
    if (pid < 0) {
        printf("Failed to fork: %s\n", strerror(errno));
    } 
    else if (!pid) {
        puts("execvp");
        // execute vector + search in PATH
        args[0] = "ls";
        const int rc = execvp("ls", (char**) args);
        if (rc < 0) {
            printf("Failed to execute command: %s\n", strerror(errno));
        }
        return 0;
    }
    if (waitpid(pid, &p_rc, 0) < 0) {
        printf("Failed to wait for pid %d: %s\n", pid, strerror(errno));
    }
    

    pid = fork();
    if (pid < 0) {
        printf("Failed to fork: %s\n", strerror(errno));
    } 
    else if (!pid) {
        puts("execvp");
        // execute vector + provide environment variables
        args[0] = "/usr/bin/ls";
        const int rc = execve("/usr/bin/ls", (char**) args, (char**) env_args);
        if (rc < 0) {
            printf("Failed to execute command: %s\n", strerror(errno));
        }
        return 0;
    }
    if (waitpid(pid, &p_rc, 0) < 0) {
        printf("Failed to wait for pid %d: %s\n", pid, strerror(errno));
    }
    
    return 0;
}
