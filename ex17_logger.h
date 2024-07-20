
#ifndef H_LOGGER_
#define H_LOGGER_

#define ERR_TIME_FAIL       -1
#define ERR_BAD_LOGGER      -2

#define INFO                "[INFO_]: "
#define DEBUG               "[DEBUG]: "
#define WARN                "[WARN_]: "
#define ERROR               "[ERROR]: "
#define FATAL               "[FATAL]: "

typedef struct Logger Logger_t;

Logger_t* logger_init(const char* const path_log_file);

void logger_clean(Logger_t** const logger);


#define DGD_LOG_INFO(logger, message)	logger_log(logger, INFO, message, __FILE__, __LINE__, __func__)
#define DGD_LOG_DEBUG(logger, message)	logger_log(logger, DEBUG, message, __FILE__, __LINE__, __func__)
#define DGD_LOG_WARN(logger, message)	logger_log(logger, WARN, message, __FILE__, __LINE__, __func__)
#define DGD_LOG_ERROR(logger, message)	logger_log(logger, ERROR, message, __FILE__, __LINE__, __func__)
#define DGD_LOG_FATAL(logger, message)	logger_log_fatal(logger, message, __FILE__, __LINE__, __func__)

size_t logger_log(Logger_t* const logger, const char* const tag, const char* const message, const char* const filename, int line, const char* const func_name);

size_t logger_log_fatal(Logger_t* const logger, const char* const message, const char* const filename, int line, const char* const func_name);

#endif
