/****************************************************************************/
/*  vproweather                                                             */
/*  A program for acquiring data from a Davis Vantage Pro Weather Station.  */
/*                                                                          */
/*  Thanks to Paul Davis for his 'remserial' program, and Aaron Logue's     */
/*  'weastat' program.                                                      */
/*                                                                          */
/* (c)2013 Joe Jaworski    email: jj@joejaworski.com                        */
/* VPROWEATHER is covered under the GNU general license. See the attached   */
/* LICENSE file for details.                                                */
/*                                                                          */
/* This software is provided as-is, without any expressed or implied        */
/* warranties, including, without limitation, the implied warranties of     */
/* merchantibility and fitness for any particular purpose.                  */
/*                                                                          */
/* main.c - main entry and intitialization                                  */
/* tabs every 4 places                                                      */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
#define _XOPEN_SOURCE
#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>
#include <time.h>
#include "main.h"
#include "names.h"
#include "dhandler.h"
#include "byte.h"

#define VERSION "1.3.5"

/* local Data */
static char szttyDevice[255];           /* Serial device string */
static bool bVerbose;                   /* Verbose Mode */
static bool bDebug;                     /* Debug Mode */
static bool bBKLOn;                     /* Backlite ON */
static bool bBKLOff;                    /* Backlite OFF */
static bool bVer;                       /* Davis firmware version astring */
static bool bModel;                     /* model name/number */
static bool bGetRTD;                    /* Get Real Time Data */
static bool bGetHLD;                    /* Get High Low Data */
static bool bGetGD;                     /* Get graph data */
static bool bArchive;                   /* Get archive data */
static bool bPrintArchiveHeader;        /* Print archive header with data */
static bool bGetInterval;               /* Get arcive interval */
static bool bGetTime;                   /* Get time flag */
static bool bSetTime;                   /* Set time flag */
static bool bHTML;                      /* html substitution flag */
static int8_t yDelay;                   /* Read wait time */
static uint16_t archiveRecords = 0;     /* Number of archive records to print */
static char datetimeString[7] = { '\0' };

static char szSerBuffer[4200];          /* serial read/write buffer */
static char szSWriteErr[] = "Serial Port Write Error";

int fdser;                      /* serial device fd */
char ch;

/* local functions */
static int GetParms(int argc, char *argv[]);
static int WakeUp(int nfd);
static int ReadNextChar(int fdser, char *pChar);
static void Delay(int secs, long microsecs);
static int ReadToBuffer(int fdser, char *pszBuffer, int nBufSize);
static int runCommand(char* command, int commandLength, int expectedLength,
        char* dataLabel, bool expectingAck, bool expectingCrc);

int main(int argc, char *argv[])
{
    struct termios oldtio, newtio;  /* serial device configuration */
    struct tm stm;
    time_t tt;
    int16_t i;
    int16_t nCnt;
    bool gotV2Data = true;
    char *nextCommand;
    uint8_t errors = 0;
    uint16_t numPages = 0;

    /* Get command line parms */
    if (!GetParms(argc, argv)) {
        printf("vproweather v%s\n", VERSION);
        printf("https://github.com/bytesnz/vproweather\n");
        printf("Original work by Joe Jaworski http://www.joejaworski.com/weather/\n");
        printf("\nUsage: vproweather [Options] Device\n");
        printf("Options:\n");
        printf(" -x, --get-realtime     Get real time weather data.\n");
        printf(" -l, --get-highlow      Get Highs/Lows data.\n");
        printf(" -g, --get-graph        Get graph data.\n");
        printf(" -a, --get-archive=arc  Print either:\n");
        printf("                          - all the archive records (default)\n");
        printf("                          - all the records since arc date\n");
        printf("                            (in yyyy-mm-dd[THH:MM format]\n");
        printf("                          - the last arc records\n");
        printf(" -A, --archive-header   Print the archive data header with the\n");
        printf("                        archive records\n");
        printf(" -i, --get-interval     Get the current archive interval\n");
        printf(" -t, --get-time         Get weather station time.\n");
        printf(" -s, --set-time         Set weather station time to system time.\n");
        printf(" -o, --bklite-on        Turn backlite on.\n");
        printf(" -f, --bklite-off       Turn backlite off.\n");
        printf(" -r, --version          Query for Davis firmware version string.\n");
        printf(" -m, --model            Query for weather station model name.\n");
        printf(" -d, --delay=num        Cmd Delay in 1/10ths seconds. Default is 10 (1 sec).\n");
        printf(" -e, --hex              Display communication between weather \n");
        printf("                        station and computer and debugging output\n");
        printf(" -v, --verbose          Verbose mode.\n");
        printf(" Device                 Serial Device. Required parameter.\n");
        printf("\n");
        printf("Examples:\n");
        printf("vproweather --get-realtime /dev/ttyp0 > rtwdata.txt\n");
        printf("Gets real time data set to file rtwdata.txt from serial device ttyp0\n");
        printf("vproweather --verbose --bklite-on /dev/ttyp0\n");
        printf("Turns the LCD backlite On, illuminating the display.\n");
        exit(2);
    }

    if(bVerbose)
        printf("Opening Serial Port %s...\n", szttyDevice);
    fdser = open(szttyDevice, O_RDWR | O_NOCTTY );
    if (fdser < 0) {
        perror("vproweather: Problem opening serial device, check device name.");
        exit(2);
    }

    tcgetattr(fdser, &oldtio);  /* save previous port settings */

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNBRK | IGNPAR;
    newtio.c_oflag = 0;
    /* set serial device input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME]    = yDelay; /* timeout in 1/10 sec intervals */
    newtio.c_cc[VMIN]     = 0;      /* block until char or timeout */
    cfsetospeed (&newtio, B19200);
    cfsetispeed (&newtio, B19200);

    if(tcsetattr(fdser, TCSAFLUSH, &newtio)) {
        perror("vproweather: Problem configuring serial device, check device name.");
        exit(2);
    }

    if(tcflush(fdser, TCIOFLUSH)) {
        perror("vproweather: Problem flushing serial device, check device name.");
        exit(2);
    }


    /* wake up the station */
    if(bVerbose)
        printf("Waking up weather station...\n");
    if(!WakeUp(fdser)) {
        fprintf(stderr, "vproweather: Can't wake up weather station- no response.\n");
        exit(2);
    }


    /* ---------------------------- */
    /* Process Command line actions */
    /* ---------------------------- */

    /* turn on backlite */
    if(bBKLOn) {
        if(bVerbose)
            printf("Turning Backlite ON...\n");
        while(ReadNextChar(fdser, &ch));    /* clear channel and delay */
        while(ReadNextChar(fdser, &ch));    /* clear channel and delay */
        strcpy(szSerBuffer, "LAMPS 1\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {   perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
    }

    /* turn off backlite */
    if(bBKLOff) {
        if(bVerbose)
            printf("Turning Backlite OFF...\n");
        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "LAMPS 0\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {   perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
    }

    /* Get firmware version strings */
    if(bVer) {
        if(bVerbose)
            printf("Getting firmware string...\n");
        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "VER\n");       /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {   perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
        Delay(1, 0L);
        nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
        if(bVerbose)
            printf("Got %d characters\n", nCnt);
        *(szSerBuffer+nCnt) = '\0';
        printf("Version Info: %s\n", szSerBuffer);
    }

    /* Get model number */
    if(bModel) {
        if(bVerbose)
            printf("Getting model number code byte...\n");
        while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
        strcpy(szSerBuffer, "WRD\022\115\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }

        tcdrain(fdser);

        if(!ReadNextChar(fdser, &ch)) {
            fprintf(stderr, "vproweather: No response to model number query.\n");
            exit(2);
        }
        if(!ReadNextChar(fdser, &ch)) {
            fprintf(stderr, "vproweather: Model number byte not sent.\n");
            exit(2);
        }
        printf("Model: Davis ");
        switch((int)ch) {
            case 0: printf("Wizard III\n");                 break;
            case 1: printf("Wizard II\n");                  break;
            case 2: printf("Monitor\n");                    break;
            case 3: printf("Perception\n");                 break;
            case 4: printf("GroWeather\n");                 break;
            case 5: printf("Energy Environmonitor\n");      break;
            case 6: printf("Health Environmonitor\n");      break;
            case 16: printf("Vantage Pro\n");               break;
            default: printf("Unknown model (%d)\n", (int)ch);   break;
        }
        if(ch != 16)
            printf("Warning: This program may not work correctly with your model.\n");
    }


    /* Get weather station archive interval */
    if (bGetInterval) {
        if (runCommand("EEBRD 2D 01\n", -1, 3, "archive interval",true, true)) {
          exit(2);
        }

        printf("archiveTime = %d\n", (uint8_t)szSerBuffer[1]);
    }

    /* Get weather station time */
    if(bGetTime) {
        if (runCommand("GETTIME\n", -1, 8, "time", true, true)) {
          exit(2);
        }

        PrintTime(szSerBuffer);                 /* ...and to stdout */
    }


    /* Set weather station time */
    if(bSetTime) {
        if(bVerbose)
            printf("Setting weather station time...\n");
        while(ReadNextChar(fdser, &ch));    /* clear channel and delay */
        strcpy(szSerBuffer, "SETTIME\n");   /* make Davis cmd string */
        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }
        ReadNextChar(fdser, &ch);           /* get ACK */
        if(ch != ACK) {
            fprintf(stderr, "vproweather: Failed to get ACK.\n");
            exit(2);
        }
        else if(bVerbose)
            printf("Received ACK, sending system time...\n");


        time(&tt);
        stm = *localtime(&tt);              /* get system time */
        stm.tm_mon  += 1;

        szSerBuffer[0] = (char)stm.tm_sec;
        szSerBuffer[1] = (char)stm.tm_min;
        szSerBuffer[2] = (char)stm.tm_hour;
        szSerBuffer[3] = (char)stm.tm_mday;
        szSerBuffer[4] = (char)stm.tm_mon;
        szSerBuffer[5] = (char)stm.tm_year;
        i = CheckCRC(6, szSerBuffer);       /* generate CRC */
        szSerBuffer[6] = HIBYTE(i);         /* send MSB first */
        szSerBuffer[7] = LOBYTE(i);

        if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
        {
            perror(szSWriteErr);
            exit(2);
        }
        tcdrain(fdser);
        Delay(1, 0L);

        ReadNextChar(fdser, &ch);           /* get ACK from console */
        if(ch != 0x06) {
            fprintf(stderr, "vproweather: CRC Failed, didn't get ACK (%d)\n", ch);
            exit(2);
        }
        else if(bVerbose)
            printf("Received ACK, CRC good, time is now set.\n");
    }



    /* Get highs/lows data set */
    if(bGetHLD) {
        if(runCommand("HILOWS\n", -1, 438, "hi/lows", true, true)) {
          exit(2);
        }

        GetHLData(szSerBuffer);             /* get data to struct */
        PrintHLData();                      /* ...and to stdout */
    }



    /* Get Graph data sets */
    if(bGetGD) {
        if (runCommand("GETEE\n", -1, 4098, "graph", true, true)) {
          exit(2);
        }

        PrintGDData((uint8_t*)szSerBuffer);         /* ...and to stdout */
    }



    /* Get real time data set (Davis LOOP data) */
    if(bGetRTD) {
        /* Get LOOP 1 data */
        if (runCommand("LOOP 1\n", -1, 99, "real time v1", true, true)) {
          exit(2);
        }
        GetRTData(szSerBuffer);             /* get data to struct */

        /* Get LOOP 2 data */
        if (runCommand("LPS 2 1\n", -1, 99, "real time v2", true, true)) {
          gotV2Data = false;
        } else {
          GetRT2Data(szSerBuffer);             /* get data to struct */
        }

        PrintRTData(gotV2Data);                      /* ...and to stdout */
    }



    if (bArchive) {
      if (runCommand("DMPAFT\n", -1, 0, "archive download initiation", true, false)) {
        exit(2);
      }

      datetimeString[6] = '\n';
      if (runCommand(datetimeString, 6, 6, "archive download initiation", true, true)) {
        exit(2);
      } else {
        StoreDownloadInfo(szSerBuffer);
        numPages = GetNumberOfPages();
        if (bVerbose) {
          PrintDownloadInfo();
        }
      }

      if (numPages) {
        if (bPrintArchiveHeader) {
          PrintArchHeader();
        }

        if (archiveRecords && (archiveRecords / 5) < numPages) {
          numPages = archiveRecords / 5;
        }

        if (bVerbose) {
          printf("%d pages will be downloaded (%d archive records)\n", numPages, archiveRecords);
        }

        i = 0;

        nextCommand = ACK_STR;
        while (errors < 5) {
          if (runCommand(nextCommand, -1, 267, "archive packet", false, true)) {
            nextCommand = NAK_STR;
            errors++;
            Delay(yDelay, 0);
          } else {
            nextCommand = ACK_STR;
            StoreArchPacket(szSerBuffer);
            PrintArchPacket((archiveRecords ? archiveRecords % 5 : 0));
            if (archiveRecords) {
              archiveRecords = archiveRecords - 5;

              // Exit if printed enough records
              if (archiveRecords <= 0) {
                break;
              }
            } else {
              if (++i >= numPages) {
                break;
              }
            }
          }
        }

        if (errors >= 5) {
          printf("Encountered more than 5 errors while downloading, so stopping\n");
          printf("Try increasing the delay\n");
        }
      }

      runCommand(ESC_STR, -1, 0, "Cancel", false, false);
    }



    /* all done, exit */
    tcsetattr(fdser, TCSANOW, &oldtio); /* restore previous port settings */
    if(close(fdser)) {
        perror("vproweather: Problem closing serial device, check device name.");
        exit(2);
    }



    exit(0);
}





/**
 * Reads command line parameters.
 *
 * @param argc Number of arguments
 * @param argv Argument string array
 *
 * @returns 0 on error (eg a parameter missing) and -1 on success
 */
int GetParms(int argc, char *argv[])
{
    extern char *optarg;
    char *endptr;
    extern int optind, opterr, optopt;
    int c, i;
    struct tm *archiveTime = malloc(sizeof(struct tm));
    memset(archiveTime, 0, sizeof(struct tm));

     /* options descriptor */
    static struct option longopts[] = {
        { "bklite-on",      no_argument,    0,  'o' },
        { "bklite-off",     no_argument,    0,  'f' },
        { "version",        no_argument,    0,  'r' },
        { "model",          no_argument,    0,  'm' },
        { "get-realtime",   no_argument,    0,  'x' },
        { "get-hilo",       no_argument,    0,  'l' },
        { "get-graph",      no_argument,    0,  'g' },
        { "get-archive",    optional_argument, 0,  'a' },
        { "archive-header", no_argument,    0,  'A' },
        { "get-interval",   no_argument,    0,  'i' },
        { "get-time",       no_argument,    0,  't' },
        { "set-time",       no_argument,    0,  's' },
        { "verbose",        no_argument,    0,  'v' },
        { "hex",            no_argument,    0,  'e' },
        { "delay",          required_argument,  0,  'd' },
        { NULL,             0,              NULL,   0 }
    };



    /* Set command line defaults */
    *szttyDevice = '\0';
    bBKLOn = false;
    bBKLOff = false;
    bVer = false;
    bModel = false;
    bGetRTD = false;
    bGetHLD = false;
    bGetGD = false;
    bArchive = false;
    bPrintArchiveHeader = false;
    bHTML = false;
    bVerbose = false;
    bDebug = false;
    bGetInterval = false;
    bGetTime = false;
    bSetTime = false;
    yDelay = 10;

    if(argc == 1)
        return 0;               /* no parms at all */

    while ((c = getopt_long(argc, argv, "ofrmxlga::Aivetsb:d:", longopts, NULL )) != EOF) {
        switch (c) {
            case 'o': bBKLOn        = true; break;
            case 'f': bBKLOff       = true; break;
            case 'r': bVer          = true; break;
            case 'm': bModel        = true; break;
            case 'x': bGetRTD       = true; break;
            case 'l': bGetHLD       = true; break;
            case 'g': bGetGD        = true; break;
            case 'i': bGetInterval  = true; break;
            case 'v': bVerbose      = true; break;
            case 't': bGetTime      = true; break;
            case 's': bSetTime      = true; break;
            case 'e':
                bVerbose = true;
                bDebug = true;
                break;
            case 'A': bPrintArchiveHeader = true; break;

            case 'a':
                bArchive = true;
                if (optarg == NULL) {
                  archiveRecords = 0;
                } else {
                  // Try and parse a full date time string
                  if (strptime(optarg, "%Y-%m-%dT%H:%M", archiveTime) == NULL
                      && strptime(optarg, "%Y-%m-%d", archiveTime) == NULL) {
                    free(archiveTime);
                    archiveTime = NULL;
                    archiveRecords = strtol(optarg, &endptr, 10);

                    if (*endptr != '\0' || endptr == optarg || archiveRecords < 0) {
                        fprintf(stderr, "vproweather: Illegal date or number of archive records to download specified.\n");
                        return 0;
                    }
                  }

                  MakeVantageDatetime(archiveTime, datetimeString);
                  GenerateCRC(4, datetimeString, &datetimeString[4]);
                  if (archiveTime != NULL) {
                    free(archiveTime);
                  }
                }
                break;

            case 'd':
                /* Get delay time */
                i = atoi(optarg);
                if(i < 0 || i > 255) {
                    fprintf(stderr, "vproweather: Illegal delay specified.\n");
                    return 0;
                }
                yDelay = (unsigned char)i;
                break;
            case '?': /* user entered unknown option */
            case ':': /* user entered option without required value */
            case 'h': /* User wants Usage message */
                return 0;

            default:
                break;
        }

        if (optind < argc)          /* get serial device name */
            strcpy(szttyDevice, argv[optind]);
        else {
            fprintf(stderr, "vproweather: No serial device specified\n");
            return 0;
        }
    }


    return -1;
}



/**
 * Wakes up the weather station per the Davis specs.
 *
 * @param nfd Serial line file descriptor
 *
 * @returns -1 on successful wake up or 0 on error
 */
int WakeUp(int nfd)
{
    char ch;
    int i;

    for(i = 0; i < 3; i++)
    {
        ch = '\r';
        /* send wake up char */
        if(write(nfd, &ch, 1) != 1)
        {
            perror(szSWriteErr);
            exit(2);
        }
        if(ReadNextChar(nfd, &ch))      /* read a char */
        {
            if(bVerbose)
                printf("Weather station woke up after %d retries\n", i+1);
            return -1;                  /* okay, its awake now */
        }
    }

    return 0;                           /* failed to wake up */
}




/**
 * Reads the next character from the serial device. Returns zero if no
 * character was available.
 *
 * @param nfd Serial line file descriptor
 * @param pChar Location to put read character
 *
 * @returns The number of characters read
 */
int ReadNextChar(int nfd, char *pChar)
{
    int nResult;

    nResult = read(nfd, pChar, 1);
    if(nResult == -1) {
        perror("vproweather: Problem reading serial device.");
        exit(2);
    }
    return nResult;
}




/**
 * Delays by the number of seconds and microseconds.
 *
 * @param secs Number of seconds to delay by
 * @param microsecs Number of microseconds to delay
 */
void Delay(int secs, long microsecs)
{
    static struct timeval t1;

    t1.tv_sec = (long)secs;
    t1.tv_usec = microsecs;
    if ( select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &t1)  < 0 )
         perror("Internal error: error in select()");
    return;
}




/**
 * Reads data to a buffer until no more characters are available. If
 * the buffer overflows, returns -1. Otherwise, returns the number of
 * characters read.
 *
 * @param nfd Serial line file descriptor
 * @param pszBuffer Buffer to put read data into
 * @param nBufSize Maximum number of characters to read
 *
 * @returns The number of characters read
 */
int ReadToBuffer(int nfd, char *pszBuffer, int nBufSize)
{
    int nPos = 0;               /* current character position */
    char *pBuf = pszBuffer;

    while(nPos < nBufSize) {
        if(!ReadNextChar(nfd, pBuf++))
            return nPos;        /* no character available */
        ++nPos;
    }
    return -1;                  /* buffer overflow */
}




/**
 * Run a command and check CRC of the reply. If the command is run
 * successfully and the data of the CRC passes, returns 0, otherwise -1
 *
 * @param command Command to run
 * @param commandLength Length (number of characters) of command
 * @param expectedLength The expected number of characters in the response
 *   (including the CRC, but excluding the ACK)
 * @param dataLabel Label for verbose messages
 * @param expectingAck Whether or not the first character should be an ACK
 * @param expectingCrc Whether or not a CRC is expected in the response
 *
 * @returns 0 on successful running of command (ACK received and CRC passed
 *   etc) or -1 on error
 */
int runCommand(char* command, int commandLength, int expectedLength, char* dataLabel,
        bool expectingAck, bool expectingCrc)
{
    int16_t nCnt;
    int i = 0;
    int max;
    int totalLength = expectedLength;

    if (commandLength == -1) {
      commandLength = strlen(command);
    }

    if(bVerbose) {
        printf("Sending %s...\n", dataLabel);
    }

    if (bDebug) {
        printf("> ");

        for (i = 0; i < commandLength; i++) {
          printf("%02x  ", (uint8_t)command[i]);
        }
        printf("\n");
    }

    while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
    memcpy(szSerBuffer, command, commandLength);         /* make Davis cmd string */
    if(write(fdser, &szSerBuffer, commandLength) != commandLength)
    {
        perror(szSWriteErr);
        return -1;
        exit(2);
    }
    tcdrain(fdser);
    nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));

    if (expectingAck) {
      totalLength = totalLength + 1;
    }

    if (bDebug) {

      max = nCnt;

      /*if (max > 20) {
        max = 20;
      }*/

      printf("< ");

      for (i = 0; i < max; i++) {
        printf("%02x  ", (uint8_t)szSerBuffer[i]);
      }
      printf("\n");
    }

    /* Check received an ACK */
    if (expectingAck) {
        if (szSerBuffer[0] != ACK) {
          fprintf(stderr, "Didn't get ACK from weather station (got %d bytes)\n", nCnt);
          return -1;
        } else if (bVerbose) {
          printf("Get ACK reply to command\n");
        }
    }

    if(bVerbose) {
        printf("Got %d of %d characters...", nCnt, totalLength);
    }

    // Check for 0x0a0d at end
    if (nCnt == totalLength + 2 && szSerBuffer[totalLength] == 0x0a
            && szSerBuffer[totalLength + 1] == 0x0d) {
        if (bVerbose) {
            printf("Good\n");
        }
    } else {
        if (bVerbose) {
            if(nCnt != totalLength)
                printf("Bad\n");
            else
                printf("Good\n");
        }

        if(nCnt != totalLength) {
            fprintf(stderr, "vproweather: Didn't get all data. Try changing delay parameter.\n");
            return -1;
            exit(2);
        }
    }
    if (expectingCrc) {
        if((nCnt = CheckCRC(expectedLength,
                szSerBuffer + (expectingAck ? 1 : 0)))) {    /* check crc */
            fprintf(stderr, "vproweather: CRC failure %d.\n", nCnt);
            return -1;
            exit(2);
        }
        else if (bVerbose)
            printf("CRC verified good on full %s packet.\n", dataLabel);
    }

    return 0;
}
