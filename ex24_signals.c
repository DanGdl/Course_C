/*
compile: gcc -Wall -Wextra -Wpedantic ex24_signals.c -o ex24   OR: gcc ex24_signals.c -o ex24
run: ./ex24
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>     // override signal handler


void handle_signal(int signal) {
    printf("Got signal: %d\n", signal);
}

int main (void) {
    printf("Pid %d\n", getpid());

    struct sigaction sigact = { 0 };
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);

    sigact.sa_handler = handle_signal;
    if (sigaction(SIGINT, &sigact, NULL) < 0) {
        printf("Failed to setup SIGINT signal\n");
        return 0;
    }
    if (sigaction(SIGUSR1, &sigact, NULL) < 0) {
        printf("Failed to setup SIGUSR1 signal\n");
        return 0;
    }
    if (sigaction(SIGUSR2, &sigact, NULL) < 0) {
        printf("Failed to setup SIGUSR2 signal\n");
        return 0;
    }
    while (getchar() != '\n');
    return 0;
}
