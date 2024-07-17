/*
compile: gcc -Wall -Wextra -Wpedantic ex10_stdin.c -o ex10   OR: gcc ex10_stdin.c -o ex10
run: ./ex10
*/

#include <stdio.h>
#include <inttypes.h>
#include <limits.h>

int main(void) {
    int8_t i8 = 0;
    uint8_t u8 = 0;

    int16_t i16 = 0;
    uint16_t u16 = 0;

    int32_t i32 = 0;
    uint32_t u32 = 0;

    int64_t i64 = 0;
    uint64_t u64 = 0;

    char name[57] = { 0 };
    puts("What's your name?");
    // danger because can overflow buffer
    // scanf("%s", name);
    // gets(name);

    // secure version without overflow
    scanf("%56s", name);
    // gets_s(name, sizeof(name)/sizeof(name[0]));
    // fgets(name, sizeof(name)/sizeof(name[0]), stdin);

    printf("%s, Enter a number in range %d...%d:\n", name, SCHAR_MIN, SCHAR_MAX);
    scanf("%"SCNd8, &i8);

    printf("%s, Enter a number in range 0...%d:\n", name, UCHAR_MAX);
    scanf("%"SCNu8, &u8);

    printf("%s, Enter a number in range %d...%d:\n", name, SHRT_MIN, SHRT_MAX);
    scanf("%"SCNd16, &i16);

    printf("%s, Enter a number in range 0...%d:\n", name, USHRT_MAX);
    scanf("%"SCNu16, &u16);

    printf("%s, Enter a number in range %"PRId32"...%"PRIu32":\n", name, ((uint32_t)(~0)/2) + 1, ((uint32_t)(~0))/2);
    scanf("%"SCNd32, &i32);

    printf("%s, Enter a number in range 0...%u:\n", name, (uint32_t)(~0));
    scanf("%"SCNu32, &u32);

    printf("%s, Enter a number in range %"PRId64"...%"PRIu64":\n", name, (uint64_t)(~0)/2 + 1, (uint64_t)(~0)/2 );
    scanf("%"SCNd64, &i64);

    printf("%s, Enter a number in range 0...%"PRIu64":\n", name, (uint64_t)(~0));
    scanf("%"SCNu64, &u64);


    printf("Your inputs: %"PRId8", %"PRIu8", %"PRId16", %"PRIu16", %"PRId32", %"PRIu32", %"PRId64", %"PRIu64"\n",
        i8, u8, i16, u16, i32, u32, i64, u64);

    return 0;
}
