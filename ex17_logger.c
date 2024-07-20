
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <execinfo.h>

#include "ex17_logger.h"


#define DATE_BUFFER_SIZE    21 // 2022-07-15 10:40:20
#define BACK_TRACE_SIZE     30 // maybe enough


typedef struct Logger {
    FILE* p_file;
} Logger_t;


int var1 = 0;
static int var2 = 0;

static void print_lol_static(void) {
    var2 = 1;
    puts("LOL static");
}

void print_lol_nonstatic(void) {
    var2 = 2;
    puts("LOL non-static");
}

Logger_t* logger_init(const char* const path_log_file) {
    print_lol_static();
    Logger_t* logger = NULL;
    do {
        if (path_log_file == NULL || strlen(path_log_file) == 0) {
            puts("Invalid path to log file");
            break;
        }
        // check if file exists
        struct stat stats = { 0 };
        if (stat(path_log_file, &stats) == 0 && !S_ISREG(stats.st_mode)) {
            printf("File is not regular file, path %s\n", path_log_file);
            break;
        }
        
        logger = calloc(1, sizeof(*logger));
        if (logger == NULL) {
            printf("Can't allocate memory for logger\n");
            break;
        }
        logger->p_file = fopen(path_log_file, "ar");
        if (logger->p_file == NULL) {
            printf("Can't open a file, path %s\n", path_log_file);
            break;
        }
        return logger;
    } while(0);
    if (logger) free(logger);
    return NULL;
}

void logger_clean(Logger_t** const logger) {
    if (logger == NULL || *logger == NULL) {
        return;
    }
    if ((*logger)->p_file != NULL) {
        fclose((*logger)->p_file);
        (*logger)->p_file = NULL;
    }
    free(*logger);
    *logger = NULL;
}



size_t logger_log(
    Logger_t* const logger, const char* const tag, const char* const message,
    const char* const filename, int line, const char* const func_name
) {
    static unsigned long int cnt_messages = 0;
    if (logger == NULL || logger->p_file == NULL) {
        return ERR_BAD_LOGGER;
    }
    time_t now = time(NULL);
    if (now == -1) {
        return ERR_TIME_FAIL;
    }
    struct tm* ptm = localtime(&now);
    if (ptm == NULL) {
        return ERR_TIME_FAIL;
    }
    char buf[DATE_BUFFER_SIZE] = {0};
    // %F == %Y-%m-%d, %R == %HH:%MM:%S
    strftime(&buf[0], DATE_BUFFER_SIZE, "%F %T.", ptm);

    size_t consumed = 0;
    if (message == NULL || strlen(message) == 0) {
        consumed = fprintf(logger->p_file, "%s%03ld %s %s:%d %s\n", (char*) &buf, now % 1000, tag, filename, line, func_name);
    } else {
        consumed = fprintf(logger->p_file, "%s%03ld %s %s:%d %s %s\n", (char*) &buf, now % 1000, tag, filename, line, func_name, message);
    }
    cnt_messages++;
    if (cnt_messages % 10 == 0) {
        printf("Printed %lu messages\n", cnt_messages);
    }
    return consumed;
}

size_t logger_log_fatal(
    Logger_t* logger, const char* const message, const char* const filename,
    int line, const char* const func_name
) {
    if (logger == NULL || logger->p_file == NULL) {
        return ERR_BAD_LOGGER;
    }
    size_t consumed = logger_log(logger, FATAL, message, filename, line, func_name);;
    void* array[BACK_TRACE_SIZE];
    const int size = backtrace(array, BACK_TRACE_SIZE);
    char** strings = backtrace_symbols(array, size);
    if (strings != NULL) {
        for (int i = size - 1; i >= 0; i--) {
            consumed += fprintf(logger->p_file, "%s\n", strings[i]);
        }
    }
    free(strings);
    return consumed;
}
