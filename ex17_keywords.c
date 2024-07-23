/*
compile: gcc -Wall -Wextra -Wpedantic ex17_keywords.c ex17_logger.c -o ex17   OR: gcc ex17_keywords.c ex17_logger.c -o ex17
run: ./ex17

static lib:
gcc -Wall -Wextra -Wpedantic -c ex17_logger.c
ar cr liblogger.a ex17_logger.o
gcc -Wall -Wextra -Wpedantic ex17_keywords.c -o ex17_static_lib -L. liblogger.a
OR: gcc -Wall -Wextra -Wpedantic -static ex17_keywords.c -o ex17_static_lib -L. llogger

dynamic library:
gcc -Wall -Wextra -Wpedantic -fPIC -c ex17_logger.c
gcc -shared -fPIC -Wall -Wextra -Wpedantic -std=c11 -o liblogger.so ex17_logger.o
gcc -Wall -Wextra -Wpedantic ex17_keywords.c -o ex17_dynamic -L. liblogger.so
*/

#include <stdio.h>

#include "ex17_logger.h"

#define MSG_TEMPLATE "test message %d"

#define USE_LIBRARY
// #define TRY_STATIC

#ifndef USE_LIBRARY
#ifdef TRY_STATIC
extern int var2;
extern void print_lol_static(void);
#endif
extern int var1;
extern void print_lol_nonstatic(void);
#endif


int main(int argc, char** argv) {
    if (argc < 2) {
        puts("Provide a path to logfile as first parameter: ./ex17 path");
        return 0;
    }
#ifndef USE_LIBRARY
#ifdef TRY_STATIC
    var2 = 10;
    print_lol_static();
#endif
    var1 = 20;
    print_lol_nonstatic();
#endif

#ifdef SAY_HELLO
    puts("Hello User!");
#endif

    const char* const path = argv[1];
    Logger_t* logger = logger_init(path);
    if (logger == NULL) {
        printf("Failed to create logger\n");
        return 0;
    }

    char buffer[28] = { 0 };
    for (int i = 0; i < 10; i++) {
        sprintf(buffer, MSG_TEMPLATE, i);
        DGD_LOG_DEBUG(logger, buffer);
        DGD_LOG_INFO(logger, buffer);
        DGD_LOG_WARN(logger, buffer);
        DGD_LOG_ERROR(logger, buffer);

    }
    DGD_LOG_DEBUG(logger, NULL);
    DGD_LOG_DEBUG(logger, "");
    DGD_LOG_FATAL(logger, "WAAAAAGHH!!!");
    logger_clean(&logger);
    return 0;
}
