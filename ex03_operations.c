/*
compile: gcc -Wall -Wextra -Wpedantic ex03_operations.c -o ex03   OR: gcc ex03_operations.c -o ex03
run: ./ex03
*/

#include <stdio.h>

int main(void) {
    int i = 2; // signed by default
    signed int si = -7;
    
    int v = si;
    v += i;
    // OR: int v = si + i;
    printf("%d + %d = %d\n", si, i, v);

    v = si - i;
    printf("%d - %d = %d\n", si, i, v);
    printf("%d * %d = %d\n", si, i, si * i);
    // WARNING:
    printf("%d / %d = %d\n", si, i, si / i);
    printf("%d / %d = %f\n", si, i, 1.0F * si / i);

    // postfix increment: si++
    // postfix decrement: si--
    // prefix increment: ++si
    // prefix decrement: --si

    // remainder:
    v = si;
    v %= 4; // v = si % 4
    printf("%d %% %d = %d\n", si, 4, v);

    // Comparison:
    printf("%d equals to %d => %d\n", si, i, si == i);
    printf("%d not equals to %d => %d\n", si, i, si != i);
    printf("%d less then %d => %d\n", si, i, si < i);
    printf("%d less or equal to %d => %d\n", si, i, si <= i);
    printf("%d bigger then %d => %d\n", si, i, si > i);
    printf("%d bigger or equal to %d => %d\n", si, i, si >= i);

    return 0;
}
