/*
compile: gcc -Wall -Wextra -Wpedantic ex07_pointers.c -o ex07   OR: gcc ex07_pointers.c -o ex07
run: ./ex07
*/

#include <stdio.h>


int main(void) {
    int v = 0x89ABCDEF;
    int* ptr = &v;
    printf("Address %p, value %d (0x%X)\n", (void*) ptr, *ptr, *ptr);
    
    unsigned char* bytes = (unsigned char*) ptr;
    for (unsigned int i = 0; i < sizeof(v); i++) {
        printf("Address %p, value %u (0x%X)\n", (void*) (bytes + i), *(bytes + i), *(bytes + i));
    }
    unsigned short* sbytes = (unsigned short*) ptr;
    for (unsigned int i = 0; i < sizeof(v)/sizeof(*sbytes); i++) {
        printf("Address %p, value %u (0x%X)\n", (void*) (sbytes + i), *(sbytes + i), *(sbytes + i));
    }
    return 0;
}
