/*
compile: gcc -Wall -Wextra -Wpedantic ex07_pointers.c -o ex07   OR: gcc ex07_pointers.c -o ex07
run: ./ex07
*/

#include <stdio.h>


int main(void) {
    int v = 0x89ABCDEF;
    int* ptr = &v;
    printf("Address %p, value %d\n", ptr, *ptr);
    
    unsigned char* bytes = (unsigned char*) ptr;
    for (unsigned int i = 0; i < sizeof(v); i++) {
        printf("Address %p, value %u\n", bytes + i, *(bytes + i));
    }
    return 0;
}
