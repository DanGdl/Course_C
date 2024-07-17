/*
compile: gcc -Wall -Wextra -Wpedantic ex13_bitwise.c -o ex13   OR: gcc ex13_bitwise.c -o ex13
run: ./ex13
*/

// enums, structs, unions, macroses
#include <stdio.h>
#include <inttypes.h>



int main(void) {
    // bitwise
    printf("%d & %d = %d\n", 0, 0, 0 & 0);
    printf("%d & %d = %d\n", 0, 1, 0 & 1);
    printf("%d & %d = %d\n\n", 1, 1, 1 & 1);

    printf("%d | %d = %d\n", 0, 0, 0 | 0);
    printf("%d | %d = %d\n", 0, 1, 0 | 1);
    printf("%d | %d = %d\n\n", 1, 1, 1 | 1);

    printf("%d ^ %d = %d\n", 0, 0, 0 ^ 0);
    printf("%d ^ %d = %d\n", 0, 1, 0 ^ 1);
    printf("%d ^ %d = %d\n\n", 1, 1, 1 ^ 1);

    // bits inversion
    printf("~%d (0x%X) = 0x%X\n", 8, 8, ~8);

    // shift
    printf("%d << %d = %d\n", 1, 1, 1 << 1);
    printf("%d >> %d = %d\n\n", 2, 1, 2 >> 1);

    // more practical.
    #define BIT_1_ON 1
    #define BIT_2_ON 2
    #define BIT_3_ON (1<<2)
    #define BIT_4_ON (1<<3)

    unsigned char bitmask = 0;
    // turn on bits:
    bitmask = bitmask | BIT_1_ON;                               // turn on one bit
    // bitmask |= BIT_1_ON;
    printf("bitmask | BIT_1_ON = %d\n", bitmask);

    bitmask = bitmask | (BIT_1_ON | BIT_2_ON);                  // turn on few bits
    // bitmask |= (BIT_1_ON | BIT_2_ON);
    printf("bitmask | (BIT_1_ON | BIT_2_ON) = %d\n", bitmask);

    bitmask = bitmask & (~BIT_1_ON);                            // turn off one bit
    // bitmask &= ~BIT_1_ON;
    printf("bitmask & (~BIT_1_ON) = %d\n", bitmask);

    bitmask = bitmask & (~(BIT_1_ON | BIT_2_ON));                // turn off few bits
    // bitmask &= ~(BIT_1_ON | BIT_2_ON);
    printf("bitmask & (~(BIT_1_ON | BIT_2_ON)) = %d\n", bitmask);

    bitmask = bitmask ^ BIT_1_ON;                               // toggle one bit
    // bitmask ^= BIT_1_ON;
    printf("bitmask ^ BIT_1_ON = %d\n", bitmask);

    bitmask = bitmask ^ (BIT_1_ON | BIT_2_ON);                  // toggle few bits
    // bitmask ^= (BIT_1_ON | BIT_2_ON);
    printf("bitmask ^ (BIT_1_ON | BIT_2_ON) = %d\n\n", bitmask);


    // max values
    const uint8_t u8_max = ~0;
    const uint16_t u16_max = ~0;
    const uint32_t u32_max = ~0;
    const uint64_t u64_max = ~0;
    printf("Max values for: u8 %u, u16 %u, u32 %u, u64 %lu\n", u8_max, u16_max, u32_max, u64_max);

    const int8_t i8_max = ((uint8_t) ~0) >> 1;
    const int16_t i16_max = 0xFFFF >> 1;
    const int32_t i32_max = u32_max >> 1;
    const int64_t i64_max = u64_max >> 1;
    printf("Max values for: i8 %d, i16 %d, i32 %d, i64 %ld\n", i8_max, i16_max, i32_max, i64_max);

    const int8_t i8_min = i8_max + 1;
    const int16_t i16_min = i16_max + 1;
    const int32_t i32_min = i32_max + 1;
    const int64_t i64_min = i64_max + 1;
    printf("Min values for: i8 %d, i16 %d, i32 %d, i64 %ld\n", i8_min, i16_min, i32_min, i64_min);
    return 0;
}
