/*
compile: gcc -Wall -Wextra -Wpedantic ex06_macros.c -o ex06   OR: gcc ex06_macros.c -o ex06
run: ./ex06

code after preprocessor: gcc -E ex06_macros.c -o ex06_macros.i
*/

#include <stdio.h>


#define CONSTANT 3


#define SUM(a, b) ((a) + (b))
#define MULT(a, b) ((a) * (b))
#define MULT_BAD(a, b) (a * b)

#define SETUP_1

#ifdef SETUP_1
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef SETUP_2
#define SETUP_2
#endif

#if defined(SETUP_1) && defined(SETUP_2)
#undef CONSTANT
#define CONSTANT (3 + 1)
#elif defined(SETUP_2)
#undef CONSTANT
#define CONSTANT (2 * 3)
#else
#error "SETUP_1 or SETUP_2 must be defined!"
#endif


#define DIVIDE(type) \
type divide_##type(type a, type b) { \
    return a / b; \
} \


DIVIDE(float)

int main(void) {
    int i = 15;

#ifdef SETUP_1
    printf("Max value is %d\n", MAX(CONSTANT, i));
    printf("Min value is %d\n", MIN(CONSTANT, i));
#endif

    printf("Sum is %d\n", SUM(CONSTANT + 10, i + 5));
    printf("Multiplication is %d\n", MULT(CONSTANT + 10, i + 5));
    printf("Multiplication (bad) is %d\n", MULT_BAD(CONSTANT + 10, i + 5));

    printf("divide_float is %f\n", divide_float(CONSTANT + 20, i));

    // built in macros
    printf("This program is built at %s %s\n", __DATE__, __TIME__);
    printf("Code info: %s %s:%d\n", __FILE__, __func__, __LINE__);

    return 0;
}
