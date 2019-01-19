#include <stdio.h>

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
#include "sim800l.h"
#include "log.h"




//#define PR(...) printf(_ _VA_ARGS_ _)

#define debug(format, ...) fprintf (stderr, format, ## __VA_ARGS__)


int main(int argc, char **argv)
{
  LOG_NOTICE("Raspberry Pi alarm program %s started %s", "now", "!!!!");

//PR("weight = %d, shipping = $%.2f\n", 10, 11.1);
//debug ("A message %s", "ddd");

#ifdef DEBUG
  printf("Debug run\n");
#else
  printf("Release run\n");
#endif

  int status = 1;

  // Use  wiringPi pin 
  if (wiringPiSetup() == -1)
  {
    //  logger(ERROR, "Wiring Pi Setup failed");
    exit(1);
  }


  foo();
  return 0;
}
