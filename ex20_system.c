/*
compile: gcc -Wall -Wextra -Wpedantic ex20_system.c -o ex20   OR: gcc ex20_system.c -o ex20
run: ./ex20
*/

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    return system("pwd") && system("ls");
}
