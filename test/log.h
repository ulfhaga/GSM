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
    LOG_LVL_ERROR,           // 1
    LOG_LVL_WARNING,            // 2
    LOG_LVL_INFO,             // 3
    LOG_LVL_DEBUG,                // 4
    LOG_LVL_TRACE,              // 5
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


#define LOG_TRACE(message) LOG_TRACE_F(message, NULL)
#define LOG_TRACE_F(format, ...) if ( IS_LOGGING(LOG_LVL_TRACE) ) {    \
    char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                     \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
  fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_TRACE]  , __func__, __LINE__, ## __VA_ARGS__) ;};
  
#define LOG_DEBUG(message) LOG_DEBUG_F(message, NULL) 
#define LOG_DEBUG_F(format, ...) if ( IS_LOGGING(LOG_LVL_DEBUG) ) {    \
    char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                     \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
    fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_DEBUG]  , __func__, __LINE__, ## __VA_ARGS__) ;};


#define LOG_INFO(message) LOG_INFO_F(message, NULL) 
#define LOG_INFO_F(format, ...) if ( IS_LOGGING(LOG_LVL_INFO) ) {      \
  char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                       \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
  fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_INFO]  , __func__, __LINE__, ## __VA_ARGS__) ;};


#define LOG_WARNING(message) LOG_WARNING_F(message, NULL) 
#define LOG_WARNING_F(format, ...) if ( IS_LOGGING(LOG_LVL_WARNING) ) {\
    char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                     \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
  fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_WARNING]  , __func__, __LINE__, ## __VA_ARGS__) ;};

#define LOG_ERROR(message) LOG_ERROR_F(message, NULL) 
#define LOG_ERROR_F(format, ...) if ( IS_LOGGING(LOG_LVL_ERROR) ) {    \
    char log_time_stamp[MAX_LEN_TIME_MESSAGE + 1];                     \
  time_t curtime = 0;                                                  \
  time_t time_epoc = time(&curtime);                                   \
  struct tm *time_structure = localtime(&time_epoc);                   \
  strftime(log_time_stamp, MAX_LEN_TIME_MESSAGE, "%Y-%m-%d %H:%M:%S", time_structure);\
    fprintf(LOG_FP,AT format, log_time_stamp, log_level_strings[LOG_LVL_ERROR]  , __func__, __LINE__, ## __VA_ARGS__) ;};

#endif                          // __log_h__

#ifdef	__cplusplus
}
#endif
