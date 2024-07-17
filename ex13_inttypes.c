/*
compile: gcc -Wall -Wextra -Wpedantic ex13_inttypes.c -o ex13   OR: gcc ex13_inttypes.c -o ex13
run: ./ex13
*/

#include <stdio.h>
// #include <stdint.h> // included in inttypes.h
#include <inttypes.h>

int main(void) {
    int8_t i8 = -1;
    uint8_t u8 = 2;

    int16_t i16 = -3;
    uint16_t u16 = 4;

    int32_t i32 = -5;
    uint32_t u32 = 6;

    int64_t i64 = 0;
    uint64_t u64 = 0;

    printf("Type int8_t: size %lu bytes, val %"PRId8"\n", sizeof(i8), i8);
    printf("Type uint8_t: size %lu bytes, val %"PRIu8"\n", sizeof(u8), u8);

    printf("Type int16_t: size %lu bytes, val %"PRId16"\n", sizeof(i16), i16);
    printf("Type uint16_t: size %lu bytes, val %"PRIu16"\n", sizeof(u16), u16);

    printf("Type int32_t: size %lu bytes, val %"PRId32"\n", sizeof(i32), i32);
    printf("Type uint32_t: size %lu bytes, val %"PRIu32"\n", sizeof(u32), u32);

    printf("Type int64_t: size %lu bytes, val %"PRId64"\n", sizeof(i64), i64);
    printf("Type uint64_t: size %lu bytes, val %"PRIu64"\n", sizeof(u64), u64);

    return 0;
}
