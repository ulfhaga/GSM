#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __log_h__
#define __log_h__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEN_TIME_MESSAGE 25

  enum {
    LOG_LVL_NONE,               // 0
    LOG_LVL_CRITICAL,           // 1
    LOG_LVL_WARNING,            // 2
    LOG_LVL_NOTICE,             // 3
    LOG_LVL_LOG,                // 4
    LOG_LVL_DEBUG,              // 5
    LOG_LVL_NEVER               // 6
  };


#ifdef DEBUG
#define LOG_BUILD_LEVEL LOG_LVL_DEBUG
#else
#define LOG_BUILD_LEVEL LOG_LVL_DEBUG
#endif



  extern unsigned char log_run_level;

  extern const char *log_level_strings[];


#ifndef LOG_FP
#ifdef stdout
#define LOG_FP stdout
#endif
#endif

#define IS_LOGGING(level) ( (level <= LOG_BUILD_LEVEL) && (level <= log_run_level) )




#define AT "%s %s [%s:%d] "

#define LOG_DEBUG(format, ...) if ( IS_LOGGING(LOG_LVL_DEBUG) ) {      \
    char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                     \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
    fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_DEBUG]  , __func__, __LINE__, ## __VA_ARGS__) ;};

#define LOG_LOG(format, ...) if ( IS_LOGGING(LOG_LVL_DEBUG) ) {        \
    char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                     \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
  fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_DEBUG]  , __func__, __LINE__, ## __VA_ARGS__) ;};

#define LOG_NOTICE(format, ...) if ( IS_LOGGING(LOG_LVL_NOTICE) ) {    \
  char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                       \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
  fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_NOTICE]  , __func__, __LINE__, ## __VA_ARGS__) ;};


#define LOG_WARNING(format, ...) if ( IS_LOGGING(LOG_LVL_WARNING) ) {  \
    char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                     \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
  fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_WARNING]  , __func__, __LINE__, ## __VA_ARGS__) ;};

#define LOG_CRITICAL(format, ...) if ( IS_LOGGING(LOG_LVL_CRITICAL) ) {\
    char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                     \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
    fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_CRITICAL]  , __func__, __LINE__, ## __VA_ARGS__) ;};

#endif                          // __log_h__

#ifdef	__cplusplus
}
#endif
