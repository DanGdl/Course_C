/*
compile: gcc -Wall -Wextra -Wpedantic ex03_operations.c -o ex03   OR: gcc ex03_operations.c -o ex03
run: ./ex03
*/

#include <stdio.h>

int main(void) {
    int i = 2; // signed by default
    int i2 = -7;
    
    int v = i2 + i;
    printf("%d + %d = %d\n", i2, i, v);

    v = i2 - i;
    printf("%d - %d = %d\n", i2, i, v);
    printf("%d * %d = %d\n", i2, i, i2 * i);
    // WARNING:
    printf("%d / %d = %d\n", i2, i, i2 / i);
    printf("%d / %d = %f\n", i2, i, 1.0F * i2 / i);

    // remainder:
    v = i2;
    v %= 4; // v = i2 % 4
    printf("%d %% %d = %d\n", i2, 4, v);

    // short notation
    v = i2;
    v += i; // v = v + i;
    v -= i; // v = v - i;
    v *= i; // v = v * i;
    v /= i; // v = v / i;
    v %= i; // v = v % i;

    // postfix increment: i++ => use i and then add 1 to i
    // postfix decrement: i-- => use i and then substract 1 from i
    // prefix increment:  ++i => i = i + 1 or i += 1 => add 1 to i and then use i
    // prefix decrement:  --i => i = i - 1 or i -= 1 => substract 1 to i and then use i

    // Comparison:
    printf("%d equals to %d => %d\n", i2, i,            i2 == i);
    printf("%d not equals to %d => %d\n", i2, i,        i2 != i);
    printf("%d less then %d => %d\n", i2, i,            i2 < i);
    printf("%d less or equal to %d => %d\n", i2, i,     i2 <= i);
    printf("%d bigger then %d => %d\n", i2, i,          i2 > i);
    printf("%d bigger or equal to %d => %d\n", i2, i,   i2 >= i);

    // BUGS!
    unsigned int i3 = 19;
    printf("%d equals to %d => %d\n", i3, i2,           i3 == i2);
    printf("%d not equals to %d => %d\n", i3, i2,       i3 != i2);
    printf("%d less then %d => %d\n", i3, i2,           i3 < i2);
    printf("%d less or equal to %d => %d\n", i3, i2,    i3 <= i2);
    printf("%d bigger then %d => %d\n", i3, i2,         i3 > i2);
    printf("%d bigger or equal to %d => %d\n", i3, i2,  i3 >= i2);

    // NO BUGS
    printf("%d equals to %d => %d\n", i3, i2,           i2 >= 0 && i3 == (unsigned int) i2);
    printf("%d not equals to %d => %d\n", i3, i2,       i2 >= 0 && i3 != (unsigned int) i2);
    printf("%d less then %d => %d\n", i3, i2,           i2 >= 0 && i3 < (unsigned int) i2);
    printf("%d less or equal to %d => %d\n", i3, i2,    i2 >= 0 && i3 <= (unsigned int) i2);
    printf("%d bigger then %d => %d\n", i3, i2,         i2 >= 0 && i3 > (unsigned int) i2);
    printf("%d bigger or equal to %d => %d\n", i3, i2,  i2 >= 0 && i3 >= (unsigned int) i2);

    return 0;
}
