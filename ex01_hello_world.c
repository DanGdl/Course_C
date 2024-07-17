/*
compile: gcc -Wall -Wextra -Wpedantic ex01_hello_world.c -o ex01   OR: gcc ex01_hello_world.c -o ex01
run: ./ex01
*/

#include <stdio.h>

// int main(void) {                 // -> can receive arguments, but don't use them
// int main() {                     // -> can receive arguments, but don't use them
int main(int agrc, char** argv) {   // -> can receive arguments, uses them
    puts("Hello world!");
    // printf("Hello world!\n");    // formatted output
    return 0;                       // 0 -> result success
}
