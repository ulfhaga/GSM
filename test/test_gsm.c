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
//#include <ioctls.h>
#include <asm-generic/ioctls.h>


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

void serialPuts(char *s);
void serialPuts1(char *s);
bool wait_response(char *expectedResponse);

#define byte 	unsigned char
#define word 	unsigned int

byte comm_line_status;

void SetCommLineStatus(byte new_status)
{
  comm_line_status = new_status;
}

byte GetCommLineStatus(void)
{
  return comm_line_status;
}

enum comm_line_status_enum {
  // CLS like CommunicationLineStatus
  CLS_FREE,                     // line is free - not used by the communication and can be used
  CLS_ATCMD,                    // line is used by AT commands, includes also time for response
  CLS_DATA,                     // for the future - line is used in the CSD or GPRS communication
  CLS_LAST_ITEM
};



int setInTextMode();
bool reportingMode();
int setGsmMode();
bool networkRegistration();

char SendSMS(char *number_str, char *message_str);

int gFD;                        // variables connected with communication buffer


#if defined (__GLIBC__)

void delay(unsigned int howLong)
{
  struct timespec sleeper, dummy;
  sleeper.tv_sec = (time_t) (howLong / 1000);
  sleeper.tv_nsec = (long) (howLong % 1000) * 1000000;
  nanosleep(&sleeper, &dummy);
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
      (" Unable to open UART.  Ensure it is not in use by another application");
    return -1;
  }

  LOG_DEBUG_F("Baud rate %i", myBaud);
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

  tcsetattr(fd, TCSANOW | TCSAFLUSH, &options);
  //ioctl (fd, TIOCMGET, &status);
//      status |= TIOCM_DTR ;
//      status |= TIOCM_RTS ;
//      ioctl (fd, TIOCMSET, &status);


  //tcflush(fd, TCIFLUSH);
  //tcsetattr(fd, TCSANOW | TCSAFLUSH, &options);


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


void features_test()
{
#ifdef _POSIX_SOURCE
  LOG_DEBUG("_POSIX_SOURCE defined");
#endif

#ifdef _POSIX_C_SOURCE
  LOG_DEBUG_F("_POSIX_C_SOURCE defined: %ldL", (long) _POSIX_C_SOURCE);
#endif

#ifdef _ISOC99_SOURCE
  LOG_DEBUG("_ISOC99_SOURCE defined");
#endif

#ifdef _ATFILE_SOURCE
  LOG_DEBUG("_ATFILE_SOURCE defined");
#endif

#ifdef _GNU_SOURCE
  LOG_DEBUG("_GNU_SOURCE defined");
#endif

#ifdef _REENTRANT
  LOG_DEBUG("_REENTRANT defined");
#endif

#ifdef _THREAD_SAFE
  LOG_DEBUG("_THREAD_SAFE defined");
#endif

#ifdef _FORTIFY_SOURCE
  LOG_DEBUG("_FORTIFY_SOURCE defined");
#endif

}

int main(int argc, char **argv)
{
  LOG_INFO_F("Raspberry Pi alarm program %s started %s", "now", "!!!!");



#ifdef DEBUG
  LOG_DEBUG("Debug run");
#else
  LOG_DEBUG("Release run");
#endif

  int status = 1;

  // Use  wiringPi pin 
  if (wiringPiSetup() == -1)
  {
    LOG_ERROR("Wiring Pi Setup failed");
    exit(1);
  }


  serialBegin(115200);
  SetCommLineStatus(CLS_FREE);
  

  if (reportingMode() == false)
  {
        LOG_ERROR("Setting in report mode failed. ");
        exit(1);
  }
   
  
  if (networkRegistration() == false)
  {
        LOG_ERROR("Network is not registrated.");
        exit(1);
  }
  
  setInTextMode();
  setGsmMode();

  /*while (wait_response("OK") == false)
     {
     delay(1000);
     }
     * */

  //SendSMS("+46706339413", "Hello");
    SendSMS("0706339413", "Hello");
  return 0;
}

int setInTextMode()
{
  LOG_DEBUG("Begin setInTextMode");
// Transmitt
//----- TX BYTES -----
  //unsigned char tx_buffer[]="AT+CMGF=1";
  //unsigned char *p_tx_buffer;

  char *u_buffer = "AT+CMGF=1";

  //Sets the GSM Module in Text Mode
/*
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 'A';
  *p_tx_buffer++ = 'T';
  *p_tx_buffer++ = '+';
  *p_tx_buffer++ = 'C';
  *p_tx_buffer++ = 'M';
  *p_tx_buffer++ = 'G';
  *p_tx_buffer++ = 'F';
  *p_tx_buffer++ = '=';
  *p_tx_buffer++ = '1';
  *p_tx_buffer++ = '\r';
*/


  if (gFD != -1)
  {
    // char *i = "\r";
    // int len = p_tx_buffer - &tx_buffer[0];
    int len = strlen(u_buffer);
    LOG_INFO_F("Write to GSM length:%i", len);
    //  int count = write(gFD, &tx_buffer[0], len); //Filestream, bytes to write, number of bytes to write

    //  int count = write(gFD, u_buffer, strlen(u_buffer)); //Filestream, bytes to write, number of bytes to write
    serialPuts(u_buffer);


    /* if (count < 0)
       {
       LOG_DEBUG("UART TX error\n");
       }
     */

    LOG_DEBUG("End setInTextMode");
  }



//----- CHECK FOR ANY RX BYTES -----

#if 0
  if (gFD != -1)
  {
    delay(2000);
    // Read up to 255 characters from the port if they are there
    unsigned char rx_buffer[256] = { 0 };
    int rx_length = read(gFD, (void *) rx_buffer, 255); //Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length < 0)
    {
      //An error occured (will occur if there are no bytes)
      LOG_INFO("No bytes");
    }
    else if (rx_length == 0)
    {
      //No data waiting
      LOG_INFO("No data waiting");

    }
    else
    {
      // delay(1000);
      //Bytes received
      LOG_INFO_F("%i bytes read:%s:\n", rx_length, &rx_buffer[0]);
    }
  }
#endif
  wait_response("OK");
  //foo();
  return 0;
}

bool reportingMode()
{
  LOG_DEBUG("Begin reportingMode");
  char *u_buffer = "AT+CMEE=2";
  LOG_INFO_F("Send %s", u_buffer);

  if (gFD != -1)
  {
    int len = strlen(u_buffer);
    LOG_INFO_F("Write to GSM length:%i", len);
    serialPuts(u_buffer);
  }
  bool isReportingMode = wait_response("OK");
  LOG_DEBUG("End reportingMode");
  return isReportingMode;
}

int setGsmMode()
{
  LOG_DEBUG("Begin GSMMode");
  char *u_buffer = "AT+CSCS=\"GSM\"";
  LOG_INFO_F("Send %s", u_buffer);

  if (gFD != -1)
  {
    int len = strlen(u_buffer);
    LOG_INFO_F("Write to GSM length:%i", len);
    serialPuts(u_buffer);
  }
  wait_response("OK");
  LOG_DEBUG("End GSMMode");
  return 0;
}

bool networkRegistration()
{
LOG_DEBUG("Begin networkRegistration");
  char *u_buffer = "AT+CREG?";
  LOG_INFO_F("Send %s", u_buffer);

  if (gFD != -1)
  {
    int len = strlen(u_buffer);
    LOG_INFO_F("Write to GSM length:%i", len);
    serialPuts(u_buffer);
  }
  bool isNetworkRegistrated= wait_response("CREG");
  LOG_DEBUG("End networkRegistration");
  return isNetworkRegistrated;
  
}

bool wait_response(char *expectedResponse)
{
  int loop = 0;
  char message[512] = { 0 };
  bool ret = false;
  if (gFD != -1)
  {


    bool status = true;
    do
    {
      loop++;
      if (loop > 30)
      {
         LOG_INFO("No bytes received. ");
          ret = false;
          status = false;
        break;
      }
      delay(1000);
      // Read up to 255 characters from the port if they are there
      char rx_buffer[256] = { 0 };
      int rx_length = read(gFD, (void *) rx_buffer, 255);       //Filestream, buffer to store in, number of bytes to read (max)
      if (rx_length < 0)
      {
        //An error occured (will occur if there are no bytes)
        LOG_INFO("No bytes more");
      }
      else if (rx_length == 0)
      {
        //No data waiting
        LOG_INFO("No more data waiting. ");
        if (strstr(message, expectedResponse) != NULL)
        {
          ret = true;
          LOG_INFO_F("Data match %s. Data is %s:\n", expectedResponse, message);
        }
        //status = false;

      }
      else
      {
        // delay(1000);
        //Bytes received
        LOG_INFO_F("%i bytes read:%s:\n", rx_length, &rx_buffer[0]);
        strncat(message, rx_buffer, rx_length);
        if (strstr(message, expectedResponse) != NULL)
        {
          ret = true;
          status = false;
        }
      }
    }
    while (status);

  }
  LOG_INFO_F("message:%s", message);
  return ret;
}


enum rx_state_enum {
  RX_NOT_FINISHED = 0,          // not finished yet
  RX_FINISHED,                  // finished, some character was received
  RX_FINISHED_STR_RECV,         // finished and expected string received
  RX_FINISHED_STR_NOT_RECV,     // finished, but expected string not received
  RX_TMOUT_ERR,                 // finished, no character received
  // initial communication tmout occurred
  RX_LAST_ITEM
};



char SendSMS(char *number_str, char *message_str)
{
  char ret_val = -1;
  byte i;

  LOG_TRACE("Begin SendSMS");

  if (CLS_FREE != GetCommLineStatus())
    return (ret_val);
  SetCommLineStatus(CLS_ATCMD);
  ret_val = 0;                  // still not send
  // try to send SMS 3 times in case there is some problem
  for (i = 0; i < 1; i++)
  {
    // send  AT+CMGS="number_str"

    //   serialPuts("AT+CSCS=\"GSM\"");


    // serialPuts1("AT+CMGS=\"");
    // serialPuts1(number_str);
    // serialPuts1("\"\r");
    delay(5000);

    /*
       LOG_DEBUG("Send AT+CMGS=?");
       serialPuts1("AT+CMGS=?");
       delay(1);
       write(gFD, "\r", 1);
       while (wait_response("OK") == false)
       {
       LOG_DEBUG("Wait again for AT+CMGS=?");

       delay(1000);
       serialPuts1("AT+CMGS=?");
       write(gFD, "\r", 1);
       }
     */

    LOG_DEBUG_F("Send SMS Message number:%s   message:%s", number_str,
                message_str);

    serialPuts("AT+CMGS=\"0706339413\"");
  //  write(gFD, "\n", 1);
    delay(1000);

    bool resp = wait_response("CMGS");
    while (resp == false)
    {
      delay(1000);
      LOG_DEBUG("Wait for a AT+CMGS .");
      resp = wait_response("CMGS");
    }

    if (resp == true)
    {
      LOG_DEBUG("Received a CMGS");
    }



    /*while (wait_response("AT") == false){
       LOG_DEBUG("Wait again 1");
       delay(1000);
       }
       * */
    delay(2000);
    serialPuts1(message_str);
    delay(10);
    // serialPuts1("\x1A");
    write(gFD, "\x1A", 1);
    //  write(gFD,"\r",1);



    resp = wait_response("OK");
    while (resp == false)
    {
      delay(1000);
      LOG_DEBUG("Wait for a successful sending.");
      resp = wait_response("OK");
    }

    if (resp == true)
    {
      LOG_DEBUG("SMS sent successful!!!!!!!!!!!!!!!!!");
    }





    //serialPuts1("AT+CMGS=\"+46706339413\"\rHello \x1A");

    // LOG_DEBUG_F("Send SMS Message number:%s   message:%s", number_str, message_str);
    //   printf("AT+CMGS=\"+46706339413\"\rHello \x1A");
    //serialPuts1("AT+CMGS=\"+46706339413\"\rHello \x1A");
    // 1000 msec. for initial comm tmout
    // 50 msec. for inter character timeout

    //return 0;

    //ulf if (RX_FINISHED_STR_RECV == WaitRespAdd(1000, 50, ">")) 
    //delay(5000);
    //  {
    // send SMS text
    //  LOG_DEBUG_F("Send number:%s   message:%s", number_str, message_str);
    //  serialPuts(message_str);
/*
//#ifdef DEBUG_SMS_ENABLED
	// SMS will not be sent = we will not pay => good for debugging
      serialPuts(ChangeIToS(0x1b));
      if (RX_FINISHED_STR_RECV == WaitRespAdd(7000, 50, "OK")) 
	  {
//#else 
      serialPuts(ChangeIToS(0x1a));
	  //mySerial.flush(); // erase rx circular buffer
      if (RX_FINISHED_STR_RECV == WaitRespAdd(7000, 5000, "+CMGS")) 
	  {
//#endif
        // SMS was send correctly 
        ret_val = 1;
		#ifdef DEBUG_PRINT
			//DebugPrint("SMS was send correctly \r\n", 0);
		#endif
        break;
      }
      else continue;

      }
	  else 
	  {
	  // try again
	  continue;
	  }
*/
// }
  }

  SetCommLineStatus(CLS_FREE);
  LOG_TRACE("End SendSMS");
  return (ret_val);
}


void serialPuts(char *s)
{
  char *i = "\r";
  write(gFD, s, strlen(s));
  write(gFD, i, strlen(i));


#if 0

  // Read up to 255 characters from the port if they are there
  unsigned char rx_buffer[256] = { 0 };
  int rx_length = read(gFD, (void *) rx_buffer, 255);   //Filestream, buffer to store in, number of bytes to read (max)
  if (rx_length < 0)
  {
    //An error occured (will occur if there are no bytes)
    LOG_INFO("No bytes");
  }
  else if (rx_length == 0)
  {
    //No data waiting
    LOG_INFO("No data waiting");
    delay(7000000);

  }
  else
  {
    // delay(1000);
    //Bytes received
    LOG_INFO_F("%i bytes read:%s:\n", rx_length, &rx_buffer[0]);
  }
#endif

}

void serialPuts1(char *s)
{
  //char *i= "\r";
  int len = strlen(s);
  write(gFD, s, len);
#if 0

  delay(100);
  // Read up to 255 characters from the port if they are there
  unsigned char rx_buffer[256] = { 0 };

  int rx_length = read(gFD, (void *) rx_buffer, 255);   //Filestream, buffer to store in, number of bytes to read (max)
  if (rx_length < 0)
  {
    //An error occured (will occur if there are no bytes)
    LOG_INFO("No bytes");
  }
  else if (rx_length == 0)
  {
    //No data waiting
    LOG_INFO("No data waiting");
    delay(7000000);

  }
  else
  {
    // delay(1000);
    //Bytes received
    LOG_INFO_F("%i bytes read:%s:\n", rx_length, &rx_buffer[0]);
  }
#endif
}
