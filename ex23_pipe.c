/*
compile: gcc -Wall -Wextra -Wpedantic ex23_pipe.c -o ex23   OR: gcc ex23_pipe.c -o ex23
run: ./ex23
*/

#include <stdio.h>
#include <errno.h>      // errno
#include <string.h>     // strerror
#include <stdlib.h>
#include <unistd.h>     // exec
#include <sys/wait.h>   // waitpid


#define LEN_STR(x) (sizeof(x)/sizeof(x[0]) - 1)


int main(void) {
    int p_rc = 0;

    // fds[0] - to read from pipe
    // fds[1] - to write to pipe
    int fds[2] = { 0 };
    if (pipe(fds) < 0) {
        printf("Failed to open pipe: %s\n", strerror(errno));
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0) {
        printf("Failed to fork: %s\n", strerror(errno));
    } 
    else if (pid == 0) {
        close(fds[0]);          // close fds[0] (fd to receive from parent), we don't need it
        dup2(fds[1], fileno(stdout));   // redirect stdout into fds[1] (fd to send to parent)
        puts("execlp");
        // execute list (first 2 parameters must be same!) + search in PATH
        const int rc = execlp("ls", "ls", "-l", "-h", NULL);
        if (rc < 0) {
            printf("Failed to execute command: %s\n", strerror(errno));
        }
        return 0;
    }
    close(fds[1]);                  // close fds[1] (fd to sent to child), we don't need it
    dup2(fds[0], fileno(stdin));    // redirect fds[0] (fd to read from child) into stdin
    char output[512];
    while (fgets(output, LEN_STR(output), stdin)) {
        printf("%s", output);
    }

    if (waitpid(pid, &p_rc, 0) < 0) {
        printf("Failed to wait for pid %d: %s\n", pid, strerror(errno));
    }
    
    puts("POPEN");
    FILE* f = popen("ls -lh", "r");
    if (f != NULL) {
        int rc = 0;
        do {
            rc = fread(output, sizeof(output[0]), LEN_STR(output), f);
            if (rc > 0) {
                output[rc] = '\0';
                printf("%s", output);
            }
        } while(rc > 0);
        pclose(f);
    }
    return 0;
}
