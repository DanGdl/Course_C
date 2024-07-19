/*
compile: gcc -Wall -Wextra -Wpedantic ex15_files.c -o ex15   OR: gcc ex15_files.c -o ex15
run: ./ex15
*/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define LEN_ARRAY(x) (sizeof(x)/sizeof(x[0]))
#define LEN_STR(x) (sizeof(x)/sizeof(x[0])-1)


#define PATH_TXT2   "./Copy.txt"


int main(int argc, char** argv) {
    if (argc < 2) {
        perror("Please provide a path to file to read");
        return 0;
    }
    // buffered IO
    // options: r, w, a, b
    FILE* f1 = fopen(argv[1], "r");
    FILE* f2 = fopen(PATH_TXT2, "w");
    if (f1 && f2) { // if (f1 != NULL && f2 != NULL)
        int rc1 = 0;
        int rc2 = 0;
        char buffer[512] = { 0 };
        while (rc1 > 0) { // while (!feof(f1)) {
            rc1 = fread(buffer, sizeof(buffer[0]), LEN_ARRAY(buffer), f1);
            if (rc1 < 0) {
                printf("Failed to read from file: %s\n", strerror(errno));
                break;
            } else if (rc1 == 0) {
                break;
            }
            rc2 = fwrite(buffer, sizeof(buffer[0]), rc1, f2);
            if (rc2 < 0) {
                printf("Failed to write to file: %s\n", strerror(errno));
            }
        }
        fflush(f2);
        fclose(f1);
        fclose(f2);
        f1 = NULL;
        f2 = NULL;
    } else {
        printf("Failed to open files: %s\n", strerror(errno));
    }

    f1 = fopen(argv[1], "rb");
    if (f1 == NULL) {
        printf("Failed to open file: %s\n", strerror(errno));
        return 0;
    }
    fseek(f1, 0, SEEK_END); // go to the end of file
    const unsigned long int size_of_file = ftell(f1);
    printf("File has size %lu bytes\n", size_of_file);
    fseek(f1, 0, SEEK_SET); // go to the beginning of file

    int byte = 0;
    int cnt = 0;
    do {
        byte = fgetc(f1);
        if (byte >= 0) {
            cnt++;
            if (cnt == 10) {
                cnt = 0;
                printf("0x%02X\n", byte);
            } else {
                printf("0x%02X ", byte);
            }
        } else {
            printf("\n");
        }
    } while (byte != EOF);
    fclose(f1);
    f1 = NULL;



    // direct system calls
    // "/dev/full", /dev/null
    // "/dev/random" - may be blocked
    // same as getrandom() or geturandom()
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        printf("Failed to open file: %s\n", strerror(errno));
        return 0;
    }
    // fd = fileno(FILE*);
    // FILE* = fdopen(fd, ...);
    printf("FD = %d\n", fd);
    char buffer[4] = { 0 };
    int rc = read(fd, buffer, LEN_ARRAY(buffer));
    if (rc < 0) {
        printf("Failed to read from file: %s\n", strerror(rc));
    }
    printf("Random data: %d %d %d %d\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    close(fd);
    
    return 0;
}
