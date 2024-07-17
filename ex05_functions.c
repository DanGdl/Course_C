/*
compile: gcc -Wall -Wextra -Wpedantic ex05_functions.c -o ex05   OR: gcc ex05_functions.c -o ex05
run: ./ex05
*/

#include <stdio.h>


void print_number(int i) {
    printf("Number %d\n", i);
}

// forward declaration
int print_double(double d);

int main(void) {
    int i = 15;
    print_number(i);

    double f = 50.25;
    int rc = print_double(f);
    printf("rc %d\n", rc);
    return 0;
}


int print_double(double i) {
    return printf("Number %f\n", i);
}
