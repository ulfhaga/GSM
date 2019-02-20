#include "log.h"

unsigned char log_run_level = LOG_LVL_TRACE;

const char *log_level_strings[] = {
  "NONE",                       // 0
  "ERROR",                      // 1
  "WARN",                       // 2
  "INFO",                       // 3
  "DEBUG",                      // 4
  "TRACE"                       // 5
};
