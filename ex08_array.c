/*
compile: gcc -Wall -Wextra -Wpedantic ex08_array.c -o ex08   OR: gcc ex08_array.c -o ex08
run: ./ex08
*/

#include <stdio.h>


void print_array(char arr[], unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        printf("%c", arr[i]);
    }
    printf("\n");
    // puts(arr);
}

void print_array2(char* arr, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        printf("%c", arr[i]);
        // printf("%c", *(arr + i));
    }
    printf("\n");
    // puts(arr);
}


float get_avegare(int* arr, unsigned int len) {
    int result = 0;
    for (unsigned int i = 0; i < len; i++) {
        result += arr[i];
    }
    return 1.0 * result / len;
}

int main(void) {
    // array without specified size (compiler will set it)
    char arr1[] = "That's array!";

    // array for 28 char elements, filled with zeroes
    char arr2[28] = { 0 };

    // array for 10 int elements, first element is 15
    int arr3[10] = { 15 };
    
    for (unsigned int i = 1; i < (sizeof(arr3)/sizeof(arr3[0])); i++) {
        arr3[i] = i;
    }
    for (unsigned int i = 0; i < (sizeof(arr3)/sizeof(arr3[0])); i++) {
        printf("Element %d is %d\n", i, arr3[i]);
    }

    for (unsigned int i = 0; i < (sizeof(arr1)/sizeof(arr1[0])); i++) {
        arr2[i] = arr1[i];
    }
    
    print_array(arr2, sizeof(arr2)/sizeof(arr2[0]));
    print_array2(arr1, sizeof(arr1)/sizeof(arr1[0]));
    printf("Average of array's values is %f\n", get_avegare(arr3, sizeof(arr3)/sizeof(arr3[0])));
    return 0;
}
