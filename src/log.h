#ifdef	__cplusplus
extern "C" {
#endif
 
#include <stdio.h>
#include <stdlib.h>
 
enum {
	LOG_LVL_NONE, // 0
	LOG_LVL_CRITICAL, // 1
	LOG_LVL_WARNING, // 2
	LOG_LVL_NOTICE, // 3
	LOG_LVL_LOG, // 4
	LOG_LVL_DEBUG, // 5
	LOG_LVL_NEVER // 6
};
 
#ifndef LOG_BUILD_LEVEL
#ifdef NDEBUG
#define LOG_BUILD_LEVEL LVL_LOG
#else
#define LOG_BUILD_LEVEL LVL_DEBUG
#endif
#endif
 
extern unsigned char log_run_level;
 
extern const char * log_level_strings [];
 
	// The BUILD_LOG_LEVEL defines what will be compiled in the executable, in production
	// it should be set to LVL_NOTICE
 
#ifndef LOG_FP
#ifdef stdout
#define LOG_FP stdout
#endif
#endif
 
#define LOG_SHOULD_I( level ) ( level <= LOG_BUILD_LEVEL && level <= log_run_level )

#ifdef WIN32
#define arg_def ...
#define arg_use __VA_ARGS__
#else
#define arg_def arg...
#define arg_use ##arg
#endif

#define LOG(level, fmt, arg_def) do {	\
	if ( LOG_SHOULD_I(level) ) { \
		fprintf(LOG_FP, "[%s] %s:%d: " fmt "\n", log_level_strings[level], __FUNCTION__,__LINE__, arg_use); \
		fflush( LOG_FP ); \
	} \
} while(0)
 
#define LL_DEBUG( fmt, arg_def ) LOG( LOG_LVL_DEBUG, fmt, arg_use )
#define LL_LOG( fmt, arg_def ) LOG( LOG_LVL_LOG, fmt,arg_use )
#define LL_NOTICE( fmt,arg_def ) LOG( LOG_LVL_NOTICE, fmt, arg_use )
#define LL_WARNING( fmt, arg_def ) LOG( LOG_LVL_WARNING, fmt, arg_use )
#define LL_CRITICAL( fmt, arg_def ) LOG( LOG_LVL_CRITICAL, fmt, arg_use )
 
#ifdef	__cplusplus
}
#endif
