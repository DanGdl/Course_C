/*
compile: gcc -Wall -Wextra -Wpedantic ex02_types.c -o ex02   OR: gcc ex02_types.c -o ex02
run: ./ex02
*/

#include <stdio.h>

int main(void) {
    char n = 1; // sign depends on system
    char c = 'a';
    int i = 2; // signed by default
    signed int si = -7;
    unsigned int ui = 6;
    short int shi = 3;
    long int li = 4;
    long int lli = 5;
    float f = 5;
    double d = 7;
    long double ld = 9;
    
    printf("Type char: size %lu bytes, val %d\n", sizeof(n), n);                // print as number
    printf("Type char: size %lu bytes, val %c\n", sizeof(c), c);                // print as symbol
    printf("Type int: size %lu bytes, val %d\n", sizeof(i), i);
    printf("Type signed int: size %lu bytes, val %d\n", sizeof(si), si);
    printf("Type unsigned int: size %lu bytes, val %u\n", sizeof(ui), ui);
    printf("Type short int: size %lu bytes, val %hi\n", sizeof(shi), shi);
    printf("Type long int: size %lu bytes, val %ld\n", sizeof(li), li);
    printf("Type long long int: size %lu bytes, val %lld\n", sizeof(lli), lli);
    printf("Type float: size %lu bytes, val %f\n", sizeof(f), f);
    printf("Type double: size %lu bytes, val %f\n", sizeof(d), d);
    printf("Type long double: size %lu bytes, val %Lf\n", sizeof(ld), ld);
    return 0;
}
