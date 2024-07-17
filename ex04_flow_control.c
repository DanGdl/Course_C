/*
compile: gcc -Wall -Wextra -Wpedantic ex04_flow_control.c -o ex04   OR: gcc ex04_flow_control.c -o ex04
run: ./ex04
*/

#include <stdio.h>

int main(void) {
    const int end = 20;

    puts("FizzBuzz with while loop");
    int i = 0;
    while (i <= end) {
        if (i % 15 == 0) {
            puts("FizzBuzz");
        } else if (i % 5 == 0) {
            puts("Buzz");
        } else if (i % 3 == 0) {
            puts("Fizz");
        } else {
            printf("%d\n", i);
        }
        i++;
    }
    puts("FizzBuzz with do-while loop");
    i = 0;
    do  {
        if (i % 3 == 0 && i % 5 == 0) {
            puts("FizzBuzz");
        } else if (i % 5 == 0) {
            puts("Buzz");
        } else if (i % 3 == 0) {
            puts("Fizz");
        } else {
            printf("%d\n", i);
        }
        i++;
    } while (i <= end);

    puts("FizzBuzz with for loop");
    for (i = 0; i <= end; i++)  {
        if (i % 3 == 0 || i % 5 == 0) {
           switch(i % 15) {
           case 0:
                puts("FizzBuzz");
                break;
            case 10:
            case 5:
                puts("Buzz");
                break;
            case 12:
            case 9:
            case 6:
            case 3:
                puts("Fizz");
                break;
            }
        } else {
            printf("%d\n", i);
        }
    }

    puts("FizzBuzz with endless for loop");
    for (i = 0; ; i++)  {
        if (i % 15 == 0) {
            puts("FizzBuzz");
        } else if (i % 5 == 0) {
            puts("Buzz");
        } else if (i % 3 == 0) {
            puts("Fizz");
        } else if (i % 2 == 0) { // if i is even
            continue;
        } else {
            printf("%d\n", i);
        }
        if (i == end) {
            break;
        }
    }

    puts("FizzBuzz with endless while loop");
    while (1)  {
        if (i % 15 == 0) {
            puts("FizzBuzz");
        } else if (i % 5 == 0) {
            puts("Buzz");
        } else if (i % 3 == 0) {
            puts("Fizz");
        } else if (i % 2 == 0) { // if i is even
            continue;
        } else {
            printf("%d\n", i);
        }
        if (i == end) {
            break;
        }
        i++;
    }

    puts("FizzBuzz with endless do-while loop");
    do {
        if (i % 15 == 0) {
            puts("FizzBuzz");
        } else if (i % 5 == 0) {
            puts("Buzz");
        } else if (i % 3 == 0) {
            puts("Fizz");
        } else if (i % 2 == 0) { // if i is even
            continue;
        } else {
            printf("%d\n", i);
        }
        if (i == end) {
            break;
        }
        i++;
    } while (1);

    puts("FizzBuzz with endless while loop and switch");
    i = 20;
    while (i <= end) {
        switch(i % 15) {
        case 0:
            puts("FizzBuzz");
            break;
        case 10:
        case 5:
            puts("Buzz");
            break;
        case 12:
        case 9:
        case 6:
        case 3:
            puts("Fizz");
            break;
        default:
            printf("%d\n", i);
        }
        i++;
    }

    int age = 18;
    if (age >= 18) {
        puts("You can buy sigaretes");
    } else {
        puts("You can\'t buy sigaretes");
    }
    // via ternary operator
    puts(age >= 18 ? "You can buy sigaretes" : "You can\'t buy sigaretes");
    return 0;
}
