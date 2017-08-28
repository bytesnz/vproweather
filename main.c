/****************************************************************************/
/*  vproweather 1.0                                                         */
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
#include <getopt.h>
#include <time.h>
#include "main.h"
#include "names.h"
#include "dhandler.h"

#define VERSION "1.2.0"

/* local Data */
static char szttyDevice[255];           /* Serial device string */
static bool bVerbose;                   /* Verbose Mode */
static bool bBKLOn;                     /* Backlite ON */
static bool bBKLOff;                    /* Backlite OFF */
static bool bVer;                       /* Davis firmware version astring */
static bool bModel;                     /* model name/number */
static bool bGetRTD;                    /* Get Real Time Data */
static bool bGetHLD;                    /* Get High Low Data */
static bool bGetGD;                     /* Get graph data */
static bool bGetTime;                   /* Get time flag */
static bool bSetTime;                   /* Set time flag */
static bool bHTML;                      /* html substitution flag */
static int8_t yDelay;                   /* Read wait time */

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
static int runCommand(char* command, int expectedLength, char* dataLabel);

/*--------------------------------------------------------------------------
    main
----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    struct termios oldtio, newtio;  /* serial device configuration */
    struct tm stm;
    time_t tt;
    int16_t i;
    int16_t nCnt;


    /* Get command line parms */
    if (!GetParms(argc, argv)) {
        printf("vproweather v%0.2f\n", VERSION);
        printf("https://github.com/bytesnz/vproweather\n");
        printf("Original work by Joe Jaworski http://www.joejaworski.com/weather/\n");
        printf("\nUsage: vproweather [Options] Device\n");
        printf("Options:\n");
        printf(" -x, --get-realtime    Get real time weather data.\n");
        printf(" -l, --get-highlow     Get Highs/Lows data.\n");
        printf(" -g, --get-graph       Get graph data.\n");
        printf(" -t, --get-time        Get weather station time.\n");
        printf(" -s, --set-time        Set weather station time to system time.\n");
        printf(" -o, --bklite-on       Turn backlite on.\n");
        printf(" -f, --bklite-off      Turn backlite off.\n");
        printf(" -r, --version         Query for Davis firmware version string.\n");
        printf(" -m, --model           Query for weather station model name.\n");
        printf(" -d, --delay=num       Cmd Delay in 1/10ths seconds. Default is 10 (1 sec).\n");
        printf(" -v, --verbose         Verbose mode.\n");
        printf(" Device                Serial Device. Required parameter.\n");
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


    /* Get weather station time */
    if(bGetTime) {
        if (runCommand("GETTIME\n", 8, "time")) {
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
        if(ch != 0x06) {
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
        if(runCommand("HILOWS\n", 438, "hi/lows")) {
          exit(2);
        }

        GetHLData(szSerBuffer);             /* get data to struct */
        PrintHLData();                      /* ...and to stdout */
    }



    /* Get Graph data sets */
    if(bGetGD) {
        if (runCommand("GETEE\n", 4098, "graph")) {
          exit(2);
        }

        PrintGDData((uint8_t*)szSerBuffer);         /* ...and to stdout */
    }



    /* Get real time data set (Davis LOOP data) */
    if(bGetRTD) {
        /* Get LOOP 1 data */
        if (runCommand("LOOP 1\n", 99, "real time v1")) {
          exit(2);
        }
        GetRTData(szSerBuffer);             /* get data to struct */

        PrintRTData();                      /* ...and to stdout */
    }

    /* all done, exit */
    tcsetattr(fdser, TCSANOW, &oldtio); /* restore previous port settings */
    if(close(fdser)) {
        perror("vproweather: Problem closing serial device, check device name.");
        exit(2);
    }

    exit(0);
}





/*--------------------------------------------------------------------------
    GetParms
    Reads command line parameters.
----------------------------------------------------------------------------*/
int GetParms(int argc, char *argv[])
{
    extern char *optarg;
    extern int optind, opterr, optopt;
    int c, i;

     /* options descriptor */
    static struct option longopts[] = {
        { "bklite-on",      no_argument,    0,  'o' },
        { "bklite-off",     no_argument,    0,  'f' },
        { "version",        no_argument,    0,  'r' },
        { "model",          no_argument,    0,  'm' },
        { "get-realtime",   no_argument,    0,  'x' },
        { "get-hilo",       no_argument,    0,  'l' },
        { "get-graph",      no_argument,    0,  'g' },
        { "get-time",       no_argument,    0,  't' },
        { "set-time",       no_argument,    0,  's' },
        { "verbose",        no_argument,    0,  'v' },
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
    bHTML = false;
    bVerbose = false;
    bGetTime = false;
    bSetTime = false;
    yDelay = 10;

    if(argc == 1)
        return 0;               /* no parms at all */

    while ((c = getopt_long(argc, argv, "ofrmxlgvtsb:d:", longopts, NULL )) != EOF) {
        switch (c) {
            case 'o': bBKLOn  = true; break;
            case 'f': bBKLOff = true; break;
            case 'r': bVer    = true; break;
            case 'm': bModel  = true; break;
            case 'x': bGetRTD = true; break;
            case 'l': bGetHLD = true; break;
            case 'g': bGetGD  = true; break;
            case 'v': bVerbose= true; break;
            case 't': bGetTime= true; break;
            case 's': bSetTime= true; break;

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



/*--------------------------------------------------------------------------
    WakeUp
    Wakes up the weather station per the Davis specs.
----------------------------------------------------------------------------*/
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




/*--------------------------------------------------------------------------
    ReadNextChar
    Reads the next character from the serial device. Returns zero if no
    character was available.
----------------------------------------------------------------------------*/
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




/*--------------------------------------------------------------------------
    Delay
    Delays by the number of seconds and microseconds.
----------------------------------------------------------------------------*/
void Delay(int secs, long microsecs)
{
    static struct timeval t1;

    t1.tv_sec = (long)secs;
    t1.tv_usec = microsecs;
    if ( select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &t1)  < 0 )
         perror("Internal error: error in select()");
    return;
}




/*--------------------------------------------------------------------------
    ReadToBuffer
    Reads data to a buffer until no more characters are available. If
    the buffer overflows, returns -1. Otherwise, returns the number of
    characters read.
----------------------------------------------------------------------------*/
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




/*--------------------------------------------------------------------------
    RunCommand
    Run a command and check CRC of the reply. If the command is run
    successfully and the data of the CRC passes, returns 0, otherwise -1
----------------------------------------------------------------------------*/
int runCommand(char* command, int expectedLength, char* dataLabel)
{
    int16_t nCnt;

    if(bVerbose)
        printf("Getting %s data set...\n", dataLabel);

    while(ReadNextChar(fdser, &ch));        /* clear channel and delay */
    strcpy(szSerBuffer, command);         /* make Davis cmd string */
    if(write(fdser, &szSerBuffer, strlen(szSerBuffer)) != strlen(szSerBuffer))
    {
        perror(szSWriteErr);
        return -1;
        exit(2);
    }
    tcdrain(fdser);
    nCnt = ReadToBuffer(fdser, szSerBuffer, sizeof(szSerBuffer));
    if(bVerbose) {
        printf("Got %d characters...", nCnt);
        if(nCnt != (expectedLength + 1))
            printf("Bad\n");
        else
            printf("Good\n");
    }
    if(nCnt != (expectedLength + 1)) {
        fprintf(stderr, "vproweather: Didn't get all data. Try changing delay parameter.\n");
        return -1;
        exit(2);
    }
    if((nCnt = CheckCRC(expectedLength, szSerBuffer+1))) {    /* check crc */
        fprintf(stderr, "vproweather: CRC failure %d.\n", nCnt);
        return -1;
        exit(2);
    }
    else if (bVerbose)
        printf("CRC verified good on full %s packet.\n", dataLabel);

    return 0;
}
