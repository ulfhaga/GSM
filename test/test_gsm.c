#include <features.h>
#include <time.h>
#include <stdio.h>

#include <wiringPi.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <stdbool.h>


#include <fcntl.h>              //Used for UART
#include <termios.h>            //Used for UART
#include <unistd.h>             //Used for UART


/*
#include <asm-generic/termios.h>
//#include <termios.h>
#include <fcntl.h>
//#include <linux/ioctl.h>
//#include <asm-generic/ioctl.h>
//#include <asm-generic/ioctls.h>

#include "sim800l.h"
#include "log.h"
*/

#include "sim800l.h"
#include "log.h"


int gFD;                        // variables connected with communication buffer


#if defined (__GLIBC__)

void delay(unsigned int howLong)
{
  struct timespec sleeper, dummy;
  sleeper.tv_sec = (time_t) (howLong / 1000);
  sleeper.tv_nsec = (long) (howLong % 1000) * 1000000;
  nanosleep(&sleeper, &dummy);
  sleep(2L);
}
#endif
/*
* serialOpen:
*	Open and initialise the serial port, setting all the right
*	port parameters - or as many as are required - hopefully!
*********************************************************************************
*/

int serialOpen(char *device, int baud)
{
  struct termios options;
  speed_t myBaud;
  int status, fd;

  switch (baud)
  {
    case 50:
      myBaud = B50;
      break;
    case 75:
      myBaud = B75;
      break;
    case 110:
      myBaud = B110;
      break;
    case 134:
      myBaud = B134;
      break;
    case 150:
      myBaud = B150;
      break;
    case 200:
      myBaud = B200;
      break;
    case 300:
      myBaud = B300;
      break;
    case 600:
      myBaud = B600;
      break;
    case 1200:
      myBaud = B1200;
      break;
    case 1800:
      myBaud = B1800;
      break;
    case 2400:
      myBaud = B2400;
      break;
    case 9600:
      myBaud = B9600;
      break;
    case 19200:
      myBaud = B19200;
      break;
    case 38400:
      myBaud = B38400;
      break;
    case 57600:
      myBaud = B57600;
      break;
    case 115200:
      myBaud = B115200;
      break;
    case 230400:
      myBaud = B230400;
      break;

    default:
      return -2;
  }

  fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  if (fd == -1)
  {
    LOG_WARNING
      (" Unable to open UART.  Ensure it is not in use by another application",
       NULL);
    return -1;
  }
  fcntl(fd, F_SETFL, O_RDWR);
  tcgetattr(fd, &options);
  cfmakeraw(&options);
  cfsetispeed(&options, myBaud);
  cfsetospeed(&options, myBaud);

  options.c_cflag |= (CLOCAL | CREAD);
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag &= ~OPOST;

  options.c_cc[VMIN] = 0;
  options.c_cc[VTIME] = 100;

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW | TCSAFLUSH, &options);
  //  ioctl (fd, TIOCMGET, &status);
  //status |= TIOCM_DTR ;
  //status |= TIOCM_RTS ;
  //ioctl (fd, TIOCMSET, &status);
  //unsigned int usecs = 1000;
  int sleep = 1;
  wait(&sleep);


  return fd;
}

void serialBegin(int baud)
{
  gFD = serialOpen("/dev/ttyS0", baud);
}





int main(int argc, char **argv)
{
  LOG_NOTICE("Raspberry Pi alarm program %s started %s", "now", "!!!!");
#ifdef _POSIX_SOURCE
  printf("_POSIX_SOURCE defined\n");
#endif

#ifdef _POSIX_C_SOURCE
  printf("_POSIX_C_SOURCE defined: %ldL\n", (long) _POSIX_C_SOURCE);
#endif

#ifdef _ISOC99_SOURCE
  printf("_ISOC99_SOURCE defined\n");
#endif

#ifdef _ATFILE_SOURCE
  printf("_ATFILE_SOURCE defined\n");
#endif

#ifdef _GNU_SOURCE
  printf("_GNU_SOURCE defined\n");
#endif

#ifdef _REENTRANT
  printf("_REENTRANT defined\n");
#endif

#ifdef _THREAD_SAFE
  printf("_THREAD_SAFE defined\n");
#endif

#ifdef _FORTIFY_SOURCE
  printf("_FORTIFY_SOURCE defined\n");
#endif

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
    LOG_CRITICAL("Wiring Pi Setup failed", NULL);
    exit(1);
  }


  serialBegin(115200);

// Transmitt
//----- TX BYTES -----
  unsigned char tx_buffer[20];
  unsigned char *p_tx_buffer;

  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 'A';
  *p_tx_buffer++ = 'T';
  *p_tx_buffer++ = '+';
  *p_tx_buffer++ = 'C';
  *p_tx_buffer++ = 'M';
  *p_tx_buffer++ = 'G';
  *p_tx_buffer++ = 'F';
  *p_tx_buffer++ = '=';
  *p_tx_buffer++ = '?';
  *p_tx_buffer++ = '\r';



  if (gFD != -1)
  {
    int len = p_tx_buffer - &tx_buffer[0];
    LOG_NOTICE("Write to GSM length:%i", len);
    int count = write(gFD, &tx_buffer[0], len); //Filestream, bytes to write, number of bytes to write
    if (count < 0)
    {
      printf("UART TX error\n");
    }
  }

  int sleep = 1;
  wait(&sleep);

//----- CHECK FOR ANY RX BYTES -----


  if (gFD != -1)
  {
    //  while (true)
    {
      // Read up to 255 characters from the port if they are there
      unsigned char rx_buffer[256];
      int rx_length = read(gFD, (void *) rx_buffer, 255);       //Filestream, buffer to store in, number of bytes to read (max)
      if (rx_length < 0)
      {
        //An error occured (will occur if there are no bytes)
        printf("No bytes\n");

      }
      else if (rx_length == 0)
      {
        //No data waiting
        printf("No data waiting\n");
      }
      else
      {

        //Bytes received
        rx_buffer[rx_length] = '\0';
        rx_buffer[rx_length + 1] = '\0';
        printf("%i bytes read:%X\n", rx_length, rx_buffer);
        // LOG_LOG("%s",rx_buffer);
      }
    }
  }


  foo();
  return 0;
}
