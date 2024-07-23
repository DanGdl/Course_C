/*
dynamic library:
gcc -Wall -Wextra -Wpedantic -fPIC -c ex17_logger.c
gcc -shared -fPIC -Wall -Wextra -Wpedantic -std=c11 -o liblogger.so ex17_logger.o
gcc -Wall -Wextra -Wpedantic ex17_keywords_d.c -o ex17_d

run: ./ex17_d
*/

// https://www.man7.org/linux/man-pages/man3/dlopen.3.html
#include <stdio.h>
#include <errno.h>      // errors
#include <dlfcn.h>
#include <string.h>     // error code to string

#define INFO                "[INFO_]: "
#define DEBUG               "[DEBUG]: "
#define WARN                "[WARN_]: "
#define ERROR               "[ERROR]: "
#define FATAL               "[FATAL]: "

#define MSG_TEMPLATE "test message %d"


typedef struct Logger Logger_t;
typedef Logger_t* (Logger_init_t)(const char* const path_log_file);
typedef void (Logger_clean_t)(Logger_t** const logger);
typedef size_t (Logger_log_t)(Logger_t* const logger, const char* const tag, const char* const message, const char* const filename, int line, const char* const func_name);
typedef size_t (Logger_log_fatal_t)(Logger_t* const logger, const char* const message, const char* const filename, int line, const char* const func_name);

typedef struct Logging {
    Logger_t* logger;
    Logger_init_t* init;
    Logger_clean_t* clean;
    Logger_log_t* log;
    Logger_log_fatal_t* log_fatal;
} Logging_t;


int main(int argc, char** argv) {
    if (argc < 2) {
        puts("Provide a path to logfile as first parameter: ./ex17 path");
        return 0;
    }
    Logging_t logger = { 0 };

    void* lib = dlopen("./liblogger.so", RTLD_LAZY);
    if (lib == NULL) {
        printf("Failed to load liblogger: %s\n", strerror(errno));
        return 0;
    }
    *((void**) &logger.init)         = dlsym(lib, "logger_init");
    *((void**) &logger.clean)        = dlsym(lib, "logger_clean");
    *((void**) &logger.log)          = dlsym(lib, "logger_log");
    *((void**) &logger.log_fatal)    = dlsym(lib, "logger_log_fatal");

    const char* const path = argv[1];
    logger.logger = logger.init(path);
    if (logger.logger == NULL) {
        printf("Failed to create logger\n");
        dlclose(lib);
        lib = NULL;
        return 0;
    }

    char buffer[28] = { 0 };
    for (int i = 0; i < 10; i++) {
        sprintf(buffer, MSG_TEMPLATE, i);
        logger.log(logger.logger, DEBUG, buffer, __FILE__, __LINE__, __func__);
        logger.log(logger.logger, INFO, buffer, __FILE__, __LINE__, __func__);
        logger.log(logger.logger, WARN, buffer, __FILE__, __LINE__, __func__);
        logger.log(logger.logger, ERROR, buffer, __FILE__, __LINE__, __func__);
    }
    logger.log(logger.logger, DEBUG, NULL, __FILE__, __LINE__, __func__);
    logger.log(logger.logger, DEBUG, "", __FILE__, __LINE__, __func__);
    logger.log_fatal(logger.logger, "WAAAAAGHH!!!", __FILE__, __LINE__, __func__);
    logger.clean(&logger.logger);

    dlclose(lib);
    lib = NULL;
    return 0;
}
