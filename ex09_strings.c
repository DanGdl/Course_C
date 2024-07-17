/*
compile: gcc -Wall -Wextra -Wpedantic ex09_array.c -o ex09   OR: gcc ex09_array.c -o ex09
run: ./ex09
*/

#include <stdio.h>
#include <string.h>

int main(void) {
    // array without specified size (compiler will set it)
    char arr1[] = "That's array!";

    // array for 28 char elements, filled with zeroes
    char arr2[56] = { 0 };

    for (unsigned int i = 0; i < (sizeof(arr1)/sizeof(arr1[0])); i++) {
        arr2[i] = arr1[i];
    }

    // strnlen
    printf("Arr1 %s, strlen %lu, len %lu\n", arr1, strlen(arr1), sizeof(arr1)/sizeof(arr1[0]));
    printf("Arr2 %s, strlen %lu, len %lu\n", arr2, strlen(arr2), sizeof(arr2)/sizeof(arr2[0]));

    char* ptr = strstr(arr1, "array");
    if (ptr == NULL) {
        printf("Substring not found\n");
    } else {
        printf("Substring found at idx %ld\n", ptr - arr1);
    }
    ptr = strchr(arr1, 'y');
    if (ptr == NULL) {
        printf("Symbol not found\n");
    } else {
        printf("Symbol found at idx %ld\n", ptr - arr1);
    }
    // copy: stpncpy
    stpcpy(arr2 + strlen(arr2), arr1);
    printf("arr1 copied into arr2: %s\n", arr2);

    // create a duplicate: strndup
    char* duplicate = strdup(arr2);
    if (duplicate == NULL) {
        printf("Failed to duplicate string\n");
    } else {
        printf("Duplicate of \"%s\" is: \"%s\"\n", arr2, duplicate);
    }
    // compare, strncmp
    int rc = strcmp(duplicate, arr2);
    if (rc > 0) {
        printf("duplicate is bigger then arr2\n");
    } else if (rc < 0) {
        printf("duplicate is less then arr2\n");
    } else {
        printf("duplicate is equals to arr2\n");
    }
    free(duplicate);


    char* new_str = strcat(arr2 + strlen(arr2), "concatenated string");
    printf("arr2 after concatenation: %s\n", arr2);
    printf("new_str: %s\n", new_str);

    memset(arr2, 0, sizeof(arr2));
    printf("arr2 after memset: %s\n", arr2);
    // memset(arr1, 0, sizeof(arr1)); // will crash

    memcpy(arr2, arr1, sizeof(arr1));
    printf("arr2 after memcpy: %s\n", arr2);

    memmove(arr2 + strlen(arr2), arr2, strlen(arr2));
    printf("arr2 after memmove: %s\n", arr2);
    for (unsigned int i = 0; i < (sizeof(arr2)/sizeof(arr2[0])); i++) {
        printf("arr2 symbol %d is %c\n", i, arr2[i]);
    }
    return 0;
}
