#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __log_h__
#define __log_h__

#include <stdio.h>
#include <stdlib.h>

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
 
extern const char * log_level_strings [];


#ifndef LOG_FP
#ifdef stdout
#define LOG_FP stdout
#endif
#endif

#define IS_LOGGING(level) ( (level <= LOG_BUILD_LEVEL) && (level <= log_run_level) )

#define LOG_DEBUG(format, ...) if ( IS_LOGGING(LOG_LVL_DEBUG) ) {      \
  fprintf(LOG_FP,"[%s] %s:%d ", log_level_strings[LOG_LVL_DEBUG],  __FUNCTION__, __LINE__) ;\
  fprintf (LOG_FP,format "\n",## __VA_ARGS__ ) ;};

#define LOG_LOG(format, ...) if ( IS_LOGGING(LOG_LVL_DEBUG) ) {        \
  fprintf(LOG_FP,"[%s] %s:%d ", log_level_strings[LOG_LVL_DEBUG],  __FUNCTION__, __LINE__) ;\
  fprintf (LOG_FP,format "\n",## __VA_ARGS__ ) ;};

#define LOG_NOTICE(format, ...) if ( IS_LOGGING(LOG_LVL_NOTICE) ) {    \
  fprintf(LOG_FP,"[%s] %s:%d ", log_level_strings[LOG_LVL_NOTICE],  __FUNCTION__, __LINE__) ;\
  fprintf (LOG_FP,format "\n",## __VA_ARGS__ ) ;};

#define LOG_WARNING(format, ...) if ( IS_LOGGING(LOG_LVL_WARNING) ) {  \
  fprintf(LOG_FP,"[%s] %s:%d ", log_level_strings[LOG_LVL_WARNING],  __FUNCTION__, __LINE__) ;\
  fprintf (LOG_FP,format "\n",## __VA_ARGS__ ) ;};

#define LOG_CRITICAL(format, ...) if ( IS_LOGGING(LOG_LVL_CRITICAL) ) {\
  fprintf(LOG_FP,"[%s] %s:%d ", log_level_strings[LOG_LVL_CRITICAL],  __FUNCTION__, __LINE__) ;\
  fprintf (LOG_FP,format "\n",## __VA_ARGS__ ) ;};

#endif // __log_h__

#ifdef	__cplusplus
}
#endif 

