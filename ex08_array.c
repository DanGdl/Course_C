/*
compile: gcc -Wall -Wextra -Wpedantic ex08_array.c -o ex08   OR: gcc ex08_array.c -o ex08
run: ./ex08
*/

#include <stdio.h>

#define LEN_ARRAY(x) (sizeof(x)/sizeof(x[0]))

void print_array(char arr[], unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        printf("%c", arr[i]);
    }
    printf("\n");
}

void print_array2(const char* const arr, const unsigned int len) {
    if (arr == NULL || len == 0) {
        return;
    }
    for (unsigned int i = 0; i < len; i++) {
        printf("%c", *(arr + i));
    }
    printf("\n");
}


float get_avegare(const int* const arr, const unsigned int len) {
    if (arr == NULL || len == 0) {
        return 0;
    }
    int result = 0;
    for (unsigned int i = 0; i < len; i++) {
        result += arr[i];
    }
    return 1.0 * result / len;
}

int main(void) {
    // array without specified size (compiler will set it). This is immutable array!!!
    char arr1[] = "That's array!";

    // array for 28 char elements, filled with zeroes
    char arr2[28] = { 0 };

    // array for 10 int elements, first element is 15
    int arr3[10] = { 15 };
    
    for (unsigned int i = 1; i < (sizeof(arr3)/sizeof(arr3[0])); i++) {
        arr3[i] = i;
    }
    for (unsigned int i = 0; i < LEN_ARRAY(arr3); i++) {
        printf("Element %d is %d\n", i, arr3[i]);
    }

    for (unsigned int i = 0; i < LEN_ARRAY(arr1); i++) {
        arr2[i] = arr1[i];
    }
    
    print_array(arr2, LEN_ARRAY(arr2));
    print_array2(arr1, LEN_ARRAY(arr1));
    printf("Average of array's values is %f\n", get_avegare(arr3, LEN_ARRAY(arr3)));
    return 0;
}
