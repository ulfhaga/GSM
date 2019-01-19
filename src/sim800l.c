#include <stdio.h>
#include <syslog.h>
#include <wiringPi.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <stdbool.h>
 
void foo(void)
{
  setlogmask (LOG_UPTO (LOG_NOTICE));
  openlog ("sim800l", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  puts("Hello, I'm a shared library");
  
  


  
  
  

  syslog (LOG_INFO, "Shared library has been invoked");





  closelog ();
}
