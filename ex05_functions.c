/*
compile: gcc -Wall -Wextra -Wpedantic ex05_functions.c -o ex05   OR: gcc ex05_functions.c -o ex05
run: ./ex05
*/

#include <stdio.h>
#include <stdarg.h>


void print_number(int i) {
    printf("Number %d\n", i);
}

// forward declaration
int print_double(double d);

void elastic(int count, ...) {
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        int j = va_arg(args, int);
        printf("Argument %d is %d\n", i, j);
    }
    va_end(args);
}

int main(void) {
    int i = 15;
    print_number(i);

    double f = 50.25;
    int rc = print_double(f);
    printf("rc %d\n", rc);

    elastic(5, 10, 20, 30, 40, 50);
    return 0;
}


int print_double(double i) {
    return printf("Number %f\n", i);
}
