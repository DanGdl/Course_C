/*
compile: gcc -Wall -Wextra -Wpedantic ex12_data_structs.c -o ex12   OR: gcc ex12_data_structs.c -o ex12
run: ./ex12
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Array {
   unsigned long int cap;
   unsigned long int len;
   int* values;
} Array_t;


typedef struct LinkedList {
   struct LinkedList* next;
   int value;
} LinkedList_t;

// HashTable
typedef struct Set {
   unsigned long int cap;
   unsigned long int len;
   int* values;
} Set_t;


#define SIZE_LIST 10
#define SET_SCALE 1.3



unsigned long int hash_number_iteraction(unsigned long int hash) {
    hash += (hash << 10);
    hash ^= (hash >> 6);
    return hash;
}

unsigned long int hash_number_end(unsigned long int hash) {
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

unsigned long int hash_number(const char* const value, unsigned long int size) {
    if (value == NULL) {
        return 0;
    }
    unsigned long int hash = 0;
    const char* byte = (const char*) value;
    for (unsigned long int i = 0; i < size; ++i) {
        hash += *byte++;
        hash = hash_number_iteraction(hash);
    }
    return hash_number_end(hash);
}

void set_add(Set_t* const set, int item) {
    const unsigned long int hash = hash_number((char*) &item, sizeof(item));
    unsigned long int idx = hash % set->cap;
    while (set->values[idx] != 0) {
        idx++;
        idx %= set->cap;
    }
    set->values[idx] = item;
    set->len++;
}

int set_increase(Set_t* const set) {
    int* arr_old = set->values;
    const unsigned long int cap_old = set->cap;
    const int new_cap = set->cap * SET_SCALE;
    set->values = calloc(new_cap, sizeof(*set->values));
    if (set->values == NULL) {
        set->values = arr_old;
        return -1;
    }
    set->cap = new_cap;
    set->len = 0;
    for (unsigned long int i = 0; i < cap_old; i++) {
        if (arr_old[i] == 0) {
            continue;
        }
        set_add(set, arr_old[i]);
    }
    free(arr_old);
    return 0;
}

int main(void) {
    Array_t list = { 0 };
    list.values = calloc(SIZE_LIST, sizeof(*list.values));
    if (list.values == NULL) {
        puts("Failed to allocate memory for array list");
        return 0;
    }
    list.cap = SIZE_LIST;
    for (int i = 0; i < 2 * SIZE_LIST; i++) {
        if (list.len >= list.cap) {
            list.values = realloc(list.values, sizeof(*list.values) * list.cap * 2);
            if (list.values == NULL) {
                puts("Failed to reallocate memory for array list");
                return 0;
            }
            list.cap = list.cap * 2;
        }
        list.values[list.len] = i;
        list.len++;
    }
    puts("Values in array:");
    for (unsigned int i = 0; i < list.len; i++) {
        printf("%d ", list.values[i]);
    }
    printf("\n");
    free(list.values);
    memset(&list, 0, sizeof(list));


    LinkedList_t* llist = NULL;
    LinkedList_t* tmp = NULL;
    for (int i = 0; i < 2 * SIZE_LIST; i++) {
        if (llist == NULL) {
            llist = calloc(1, sizeof(*llist));
            if (llist == NULL) {
                puts("Failed to allocate memory for linked list");
                return 0;
            }
            tmp = llist;
        }
        else if (tmp->next == NULL) {
            tmp->next = calloc(1, sizeof(*tmp->next));
            if (tmp->next == NULL) {
                puts("Failed to allocate memory for linked list");
                return 0;
            }
            tmp = tmp->next;
        }
        tmp->value = i;
    }

    puts("Values in LinkedList:");
    tmp = llist;
    while(tmp != NULL) { // while(tmp)
        printf("%d ", tmp->value);
        tmp = tmp->next;
    }

    while(llist) { // while(tmp)
        tmp = llist->next;
        free(llist);
        llist = tmp;
    }
    printf("\n");


    Set_t set = { 0 };
    set.values = calloc(SIZE_LIST, sizeof(*set.values));
    if (set.values == NULL) {
        puts("Failed to allocate memory for set");
        return 0;
    }
    set.cap = SIZE_LIST;
    for (int i = 1; i < 2 * SIZE_LIST + 1; i++) {
        if (set.len * SET_SCALE >= set.cap && set_increase(&set) != 0) {
            puts("Failed to reallocate memory for set");
            return 0;
        }
        set_add(&set, i);
    }
    puts("Values in set:");
    for (unsigned int i = 0; i < set.cap; i++) {
        printf("%d ", set.values[i]);
    }
    printf("\n");
    free(set.values);
    memset(&set, 0, sizeof(set));

    return 0;
}
