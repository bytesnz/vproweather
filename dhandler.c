/****************************************************************************/
/*  vproweather 1.1                                                         */
/*  A program for acquiring data from a Davis Vantage Pro Weather Station.  */
/*                                                                          */
/*  Thanks to Paul Davis for his 'remserial' program, and Aaron Logue's     */
/*  'weastat' program.                                                      */
/*                                                                          */
/* (c)2004 Joe Jaworski    email: jj@joejaworski.com                        */
/* VPROWEATHER is covered under the GNU general license. See the attached   */
/* LICENSE file for details.                                                */
/*                                                                          */
/* This software is provided as-is, without any expressed or implied        */
/* warranties, including, without limitation, the implied warranties of     */
/* merchantibility and fitness for any particular purpose.                  */
/*                                                                          */
/*                                                                          */
/* dhandler.c - data handler functions                                      */
/* tabs every 4 places                                                      */
/*                                                                          */
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>
#include "dhandler.h"
#include "main.h"
#include "names.h"
#include "byte.h"


/* CCITT table of CRC values */
static uint16_t crc_table [] = {
0x0,     0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
0x1231,  0x210,   0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
0x2462,  0x3443,  0x420,   0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
0x3653,  0x2672,  0x1611,  0x630,   0x76d7,  0x66f6,  0x5695,  0x46b4,
0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
0x48c4,  0x58e5,  0x6886,  0x78a7,  0x840,   0x1861,  0x2802,  0x3823,
0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0xa50,   0x3a33,  0x2a12,
0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0xc60,   0x1c41,
0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0xe70,
0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
0x1080,  0xa1,    0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
0x2b1,   0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
0x34e2,  0x24c3,  0x14a0,  0x481,   0x7466,  0x6447,  0x5424,  0x4405,
0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
0x26d3,  0x36f2,  0x691,   0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x8e1,   0x3882,  0x28a3,
0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
0x4a75,  0x5a54,  0x6a37,  0x7a16,  0xaf1,   0x1ad0,  0x2ab3,  0x3a92,
0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0xcc1,
0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0xed1,   0x1ef0,
};


static char szForeStrings[] =

"Mostly clear and cooler.\0"
"Mostly clear with little temperature change.\0"
"Mostly clear for 12 hrs. with little temperature change.\0"
"Mostly clear for 12 to 24 hrs. and cooler.\0"
"Mostly clear with little temperature change.\0"
"Partly cloudy and cooler.\0"
"Partly cloudy with little temperature change.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear and warmer.\0"
"Partly cloudy with little temperature change.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 24 to 48 hrs.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds with little temperature change. Precipitation possible within 24 hrs.\0"
"Mostly clear with little temperature change.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds with little temperature change. Precipitation possible within 12 hrs.\0"
"Mostly clear with little temperature change.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 24 hrs.\0"
"Mostly clear and warmer. Increasing winds.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 12 hrs. Increasing winds.\0"
"Mostly clear and warmer. Increasing winds.\0"
"Increasing clouds and warmer.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 12 hrs. Increasing winds.\0"
"Mostly clear and warmer. Increasing winds.\0"
"Increasing clouds and warmer.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 12 hrs. Increasing winds.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly clear and warmer. Precipitation possible within 48 hrs.\0"
"Mostly clear and warmer.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds with little temperature change. Precipitation possible within 24 to 48 hrs.\0"
"Increasing clouds with little temperature change.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 12 to 24 hrs.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 12 to 24 hrs. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 12 to 24 hrs. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 6 to 12 hrs.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 6 to 12 hrs. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 12 to 24 hrs. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation possible within 12 hrs.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and warmer. Precipitation likely.\0"
"clearing and cooler. Precipitation ending within 6 hrs.\0"
"Partly cloudy with little temperature change.\0"
"clearing and cooler. Precipitation ending within 6 hrs.\0"
"Mostly clear with little temperature change.\0"
"Clearing and cooler. Precipitation ending within 6 hrs.\0"
"Partly cloudy and cooler.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear and cooler.\0"
"clearing and cooler. Precipitation ending within 6 hrs.\0"
"Mostly clear with little temperature change.\0"
"Clearing and cooler. Precipitation ending within 6 hrs.\0"
"Mostly clear and cooler.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds with little temperature change. Precipitation possible within 24 hrs.\0"
"Mostly cloudy and cooler. Precipitation continuing.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation likely.\0"
"Mostly cloudy with little temperature change. Precipitation continuing.\0"
"Mostly cloudy with little temperature change. Precipitation likely.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and cooler. Precipitation possible and windy within 6 hrs.\0"
"Increasing clouds with little temperature change. Precipitation possible and windy within 6 hrs.\0"
"Mostly cloudy and cooler. Precipitation continuing. Increasing winds.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation likely. Increasing winds.\0"
"Mostly cloudy with little temperature change. Precipitation continuing. Increasing winds.\0"
"Mostly cloudy with little temperature change. Precipitation likely. Increasing winds.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and cooler. Precipitation possible within 12 to 24 hrs. Possible wind shift to the W, NW, or N.\0"
"Increasing clouds with little temperature change. Precipitation possible within 12 to 24 hrs. Possible wind shift to the W, NW, or N.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and cooler. Precipitation possible within 6 hrs. Possible wind shift to the W, NW, or N.\0"
"Increasing clouds with little temperature change. Precipitation possible within 6 hrs. Possible wind shift to the W, NW, or N.\0"
"Mostly cloudy and cooler. Precipitation ending within 12 hrs. Possible wind shift to the W, NW, or N.\0"
"Mostly cloudy and cooler. Possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Precipitation ending within 12 hrs. Possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Possible wind shift to the W, NW, or N.\0"
"Mostly cloudy and cooler. Precipitation ending within 12 hrs. Possible wind shift to the W, NW, or N.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation possible within 24 hrs. Possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Precipitation ending within 12 hrs. Possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Precipitation possible within 24 hrs. Possible wind shift to the W, NW, or N.\0"
"clearing, cooler and windy. Precipitation ending within 6 hrs.\0"
"clearing, cooler and windy.\0"
"Mostly cloudy and cooler. Precipitation ending within 6 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Mostly cloudy and cooler. Windy with possible wind shift to the W, NW, or N.\0"
"clearing, cooler and windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy with little temperature change. Precipitation possible within 12 hrs. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and cooler. Precipitation possible within 12 hrs., possibly heavy at times. Windy.\0"
"Mostly cloudy and cooler. Precipitation ending within 6 hrs. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation possible within 12 hrs. Windy.\0"
"Mostly cloudy and cooler. Precipitation ending in 12 to 24 hrs.\0"
"Mostly cloudy and cooler.\0"
"Mostly cloudy and cooler. Precipitation continuing, possible heavy at times. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation possible within 6 to 12 hrs. Windy.\0"
"Mostly cloudy with little temperature change. Precipitation continuing, possibly heavy at times. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy with little temperature change. Precipitation possible within 6 to 12 hrs. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds with little temperature change. Precipitation possible within 12 hrs., possibly heavy at times. Windy.\0"
"Mostly cloudy and cooler. Windy.\0"
"Mostly cloudy and cooler. Precipitation continuing, possibly heavy at times. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation likely, possibly heavy at times. Windy.\0"
"Mostly cloudy with little temperature change. Precipitation continuing, possibly heavy at times. Windy.\0"
"Mostly cloudy with little temperature change. Precipitation likely, possibly heavy at times. Windy.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and cooler. Precipitation possible within 6 hrs. Windy.\0"
"Increasing clouds with little temperature change. Precipitation possible within 6 hrs. windy\0"
"Increasing clouds and cooler. Precipitation continuing. Windy with possible wind shift to the W, NW, or N.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation likely. Windy with possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Precipitation continuing. Windy with possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Precipitation likely. Windy with possible wind shift to the W, NW, or N.\0"
"Increasing clouds and cooler. Precipitation possible within 6 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and cooler. Precipitation possible within 6 hrs. Possible wind shift to the W, NW, or N.\0"
"Increasing clouds with little temperature change. Precipitation possible within 6 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Increasing clouds with little temperature change. Precipitation possible within 6 hrs. Possible wind shift to the W, NW, or N.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and cooler. Precipitation possible within 6 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Increasing clouds with little temperature change. Precipitation possible within 6 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Increasing clouds and cooler. Precipitation possible within 12 to 24 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Increasing clouds with little temperature change. Precipitation possible within 12 to 24 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Mostly cloudy and cooler. Precipitation possibly heavy at times and ending within 12 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation possible within 6 to 12 hrs., possibly heavy at times. Windy with possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Precipitation ending within 12 hrs. Windy with possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Precipitation possible within 6 to 12 hrs., possibly heavy at times. Windy with possible wind shift to the W, NW, or N.\0"
"Mostly cloudy and cooler. Precipitation continuing.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation likely, windy with possible wind shift to the W, NW, or N.\0"
"Mostly cloudy with little temperature change. Precipitation continuing.\0"
"Mostly cloudy with little temperature change. Precipitation likely.\0"
"Partly cloudy with little temperature change.\0"
"Mostly clear with little temperature change.\0"
"Mostly cloudy and cooler. Precipitation possible within 12 hours, possibly heavy at times. Windy.\0"
"FORECAST REQUIRES 3 HRS. OF RECENT DATA\0"
"Mostly clear and cooler.\0\0" ;


static RTDATA rcd;          /* the one and only v1 real time weather packet */
static RTDATA2 rcd2;          /* the one and only v2 real time weather packet */
static HLDATA hld;          /* the one and only highs/lows packet */
static ARCHINFO arcInfo;    /* archive download info packet */
static ARCHDOWNLOAD arcPacket;

/* local functions */
static char* TimeConvert(uint16_t wTime);
static void PrintTimeSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize);
static void PrintByteSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize);
static void PrintBarSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize);
static void PrintRainRateSet(uint8_t *pData, int nOffset, uint8_t yNext,
            int nSetSize);
static void PrintDate(uint16_t wDate);
static void PrintTempSet24(uint8_t *pData, int nOffset, uint8_t yNext, int nSub);
static void PrintTempSet25(uint8_t *pData, int nOffset, uint8_t yNext, int nSub);
static void PrintDateSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize);
static void PrintTimeRef(void);



/**
 * Verifies the crc against a passed buffer. Returns the non-zero CRC code
 * if error, zero if the crc passed.
 *
 * @param nCnt Number of bytes to check CRC of
 * @param pData Pointer to string containing data
 *
 * @returns Leftover checksum. Should be 0 for valid data
 */
int CheckCRC(int nCnt, char *pData)
{
    int i;
    uint16_t wCRC = 0;                  /* zero checksum to start   */
    uint8_t y;

    for( i = 0; i < nCnt; i++) {
        y = *(pData)++;
        wCRC = crc_table[(wCRC >> 8) ^ y] ^ (wCRC << 8); /* CCITT std */
    }

    return wCRC;                    /* if zero, it passed */
}



/**
 * Generate the two byte CRC string of a string to pass to the Vantage when
 * sending data to it.
 *
 * @param nCnt Number of bytes to check CRC of
 * @param pData Pointer to string containing data to generate the CRC for
 * @param crc Pointer to string to put the generated CRC
 */
void GenerateCRC(int nCnt, char *pData, char *crc)
{
  uint16_t value = CheckCRC(nCnt, pData);
  crc[0] = HIBYTE(value);
  crc[1] = LOBYTE(value);
  crc[2] = 0;
}


/**
 * Creates VantagePro date integer from the given time structure
 *
 * @param time Time struct to make date integer for
 *
 * @returns VantagePro date integer
 */
uint16_t MakeVangateDateInt(struct tm* time) {
  return time->tm_mday + ((time->tm_mon + 1) * 32) + ((time->tm_year - 100) * 512);
}


/**
 * Creates a Vantage Datetime uint16_t[2] from the given time struct
 * [ day + month*32 + (year-2000)*512, 100*hours + minutes ]
 *
 * @param time Time to generate teh Datetime form
 * @param datetimeString Pointer to string where to put the generated
 *     datetime string
 */
void MakeVantageDatetime(struct tm* time, char *datetimeString)
{
  uint16_t value = 0;

  if (time != NULL) {
    // Calculate date portion
    value = MakeVangateDateInt(time);
    datetimeString[1] = HIBYTE(value);
    datetimeString[0] = LOBYTE(value);

    //Calculate time portion
    value = time->tm_min + (100 * time->tm_hour);
    datetimeString[3] = HIBYTE(value);
    datetimeString[2] = LOBYTE(value);

    datetimeString[4] = 0;
  }
}

/**
 * Prints the time data retrieved from the weather station.
 *
 * @param szData Pointer to string containing time data to convert to
 *     time string
 */
void PrintTime(char *szData)
{
    struct tm stm;
    time_t tt;
    char szBuf[255];

    time(&tt);
    stm = *localtime(&tt);              /* fill time struct */
    stm.tm_sec  = (int)szData[1];       /* copy in Davis values */
    stm.tm_min  = (int)szData[2];
    stm.tm_hour = (int)szData[3];
    stm.tm_mday = (int)szData[4];
    stm.tm_mon  = (int)(szData[5] - 1);
    stm.tm_year = (int)szData[6];

    tt = mktime(&stm);
    stm = *localtime(&tt);              /* correct bogus values */

    /* change the following line if you don't like the time format */
    strftime(szBuf, sizeof(szBuf), "%A, %B %d, %Y  %I:%M %p\n"  , &stm);

    printf("DavisTime = %s", szBuf);
}




/**
 * Copies the Archive download packet to the static ARCHDOWNLOAD struct.
 *
 * @param szData Pointer to string buffer containing archive packet
 */
void StoreArchPacket(char *szData)
{
    memcpy((char*)&arcPacket, szData, sizeof(ARCHDOWNLOAD));
}



/**
 * Copies the Archive download info to the static ARCHINFO struct.
 *
 * @param szData Pointer to string buffer containing archive download
 *     info packet
 */
void StoreDownloadInfo(char *szData)
{
    memcpy((char*)&arcInfo, szData, sizeof(ARCHINFO));
}



/**
 * Gets the real time weather data packet to the static RTDATA struct.
 *
 * @param szData Pointer to string buffer containing realtime data packet
 */
void GetRTData(char *szData)
{
    memcpy((char*)&rcd, szData, sizeof(RTDATA));
}



/**
 * Gets the real time weather data packet v2 to the static RTDATA2 struct.
 *
 * @param szData Pointer to string buffer containing realtime data packet
 */
void GetRT2Data(char *szData)
{
    memcpy((char*)&rcd2, szData, sizeof(RTDATA2));
}




/**
 * Gets the high/low weather data packet to the static HLDATA struct.
 *
 * @param szData Pointer to string buffer containing archive packet
 */
void GetHLData(char *szData)
{
    memcpy((char*)&hld, szData, sizeof(HLDATA));
}




/**
 * Returns the Rose (N,NE,E etc) string for the given bearing.
 *
 * @param bearing Bearing in degrees to return rose string for
 *
 * @returns Rose string
 */
char* getWindRose(uint8_t bearing) {
    if(bearing >= 347 && bearing < 12)        /* compass rose version */
        return "N";
    else if(bearing >= 12 && bearing < 34)
        return "NNE";
    else if(bearing >= 34 && bearing < 57)
        return "NE";
    else if(bearing >= 57 && bearing < 79)
        return "ENE";
    else if(bearing >= 79 && bearing < 102)
        return "E";
    else if(bearing >= 102 && bearing < 124)
        return "ESE";
    else if(bearing >= 124 && bearing < 147)
        return "SE";
    else if(bearing >= 147 && bearing < 170)
        return "SSE";
    else if(bearing >= 170 && bearing < 192)
        return "S";
    else if(bearing >= 192 && bearing < 214)
        return "SSW";
    else if(bearing >= 214 && bearing < 237)
        return "SW";
    else if(bearing >= 237 && bearing < 259)
        return "WSW";
    else if(bearing >= 259 && bearing < 280)
        return "W";
    else if(bearing >= 280 && bearing < 303)
        return "WNW";
    else if(bearing >= 303 && bearing < 347)
        return "NW";
    else    /*  >326 <347 */
        return "NNW";
}

/**
 * Dumps the real time weather data to stdout.
 *
 * @param includeLoop2Data Whether or not to include data from LOOP2 packet
 */
void PrintRTData(bool includeLoop2Data)
{
    int16_t i;

    printf("%s = 0x%04x\n", _NEXT_RECORD, rcd.wNextRec );
    /* 3-hour rolling baro trend */
    i = rcd.cP;
    printf("%s = ", _BARO_TREND);
    switch(i) {
        case -60: printf("Falling Rapidly\n"); break;
        case -20: printf("Falling Slowly\n"); break;
        case 0:   printf("Steady\n"); break;
        case 20:  printf("Rising Slowly\n"); break;
        case 60:  printf("Rising Rapidly\n"); break;
        default:  printf("n/a-%d\n", rcd.cP);
    }
    printf("%s = ", _BARO_TREND_IMG);
    switch(i) {
        case -60: printf("baro_fr\n"); break;
        case -20: printf("baro_fs\n"); break;
        case 0:   printf("baro_s\n"); break;
        case 20:  printf("baro_rs\n"); break;
        case 60:  printf("baro_rr\n"); break;
        default:  printf("baro_none\n");
    }
    printf("%s = %2.2f\n", _BARO_CURR, rcd.wBarometer / 1000.0 );
    printf("%s = %.1f\n", _INSIDE_TEMP, ((int16_t)rcd.wInsideTemp) / 10.0 );
    printf("%s = %d\n", _INSIDE_HUM, rcd.yInsideHum );
    printf("%s = %.1f\n", _OUTSIDE_TEMP, ((int16_t)rcd.wOutsideTemp) / 10.0 );
    printf("%s = %d\n", _WIND_SPEED, rcd.yWindSpeed );
    if (includeLoop2Data) {
      printf("%s = %.1f\n", _WIND_AVG_SPEED, (double)rcd2.avgWindSpd10m / 10.0 );
    } else {
      printf("%s = %d\n", _WIND_AVG_SPEED, rcd.yAvgWindSpeed );
    }
    if (includeLoop2Data) printf("%s = %.1f\n", _WIND_2M_AVG_SPEED, (double)rcd2.avgWindSpd2m / 10.0 );
    printf("%s = %d\n", _WIND_DIR, rcd.wWindDir );
    printf("%s = %s\n", _WIND_DIR_ROSE, getWindRose(rcd.wWindDir) );
    if (includeLoop2Data) printf("%s = %d\n", _WIND_10M_GUST_SPEED, rcd2.windGust10m );
    if (includeLoop2Data) printf("%s = %d\n", _WIND_10M_GUST_DIR, rcd2.windGust10mDir );
    if (includeLoop2Data) printf("%s = %s\n", _WIND_10M_GUST_DIR_ROSE, getWindRose(rcd2.windGust10mDir) );
    printf("%s = %d\n", _OUTSIDE_HUM, rcd.yOutsideHum );
    printf("%s = %.2f\n", _RAIN_RATE, rcd.wRainRate / 100.0 );
    printf("%s = %s\n", _IS_RAINING, rcd.wRainRate ? "yes" : "no");
    printf("%s = ", _UV_LEVEL);
    if(rcd.yUVLevel == 0xff)
        printf("n/a\n");
    else
        printf("%.1f\n", rcd.yUVLevel / 10.0 );
    printf("%s = ", _SOLAR_RAD);
    if(rcd.wSolarRad == 32767)
        printf("n/a\n");
    else
        printf("%d\n", rcd.wSolarRad );
    if (includeLoop2Data) printf("%s = %d\n", _HEAT_INDEX, rcd2.heatIndex );
    if (includeLoop2Data) printf("%s = %d\n", _WIND_CHILL, rcd2.windChill );
    if (includeLoop2Data) printf("%s = %d\n", _THSW_INDEX, rcd2.thswIndex );
    printf("%s = %.2f\n", _RAIN_STORM, rcd.wStormRain / 100.0 );
    printf("%s = ", _STORM_START_DATE);
    PrintDate(rcd.wStormStart);
    printf("\n");

    if (includeLoop2Data) printf("%s = %.2f\n", _RAIN_LAST_15M, rcd2.last15mRain / 100.0);
    if (includeLoop2Data) printf("%s = %.2f\n", _RAIN_LAST_HOUR, rcd2.lastHourRain / 100.0);
    printf("%s = %.2f\n", _DAY_RAIN, rcd.wRainDay / 100.0);
    printf("%s = %.2f\n", _MONTH_RAIN, rcd.wRainMonth / 100.0);
    printf("%s = %.2f\n", _YEAR_RAIN, rcd.wRainYear / 100.0);
    printf("%s = %d\n", _DAY_ET, rcd.wETDay);
    printf("%s = %d\n", _MONTH_ET, rcd.wETMonth);
    printf("%st = %d\n", _XMIT_BATT, rcd.yXmitBatt);
    printf("%s = %.1f\n", _BATT_VOLTAGE, ((rcd.wBattLevel * 300)/512)/100.0);
    printf("%s = %d\n", _FORE_ICON, rcd.yForeIcon);
    printf("%s = %d\n", _FORE_RULE, rcd.yRule);
    printf("%s = %s\n", _FORE_STRING, ForecastString(rcd.yRule));
    printf("%s = %s\n", _SUNRISE, TimeConvert(rcd.wSunrise));
    printf("%s = %s\n", _SUNSET, TimeConvert(rcd.wSunset));
}



/**
 * Converts a BCD encoded time value to ascii human readable form. Returns
 * a pointer to a static buffer containing the converted ascii string. The
 * form is '12:33PM'. If you don't like this format, here is where you need
 * to change it.
 *
 * @param wTime Time integer to convert
 *
 * @returns The static time string
 */
char* TimeConvert(uint16_t wTime)
{
    static char szBuf[32];          /* static return buffer */
    bool bAM = true;
    int nHours = wTime/100;
    int nMinutes = wTime - ((wTime/100) * 100);

    if ((unsigned)wTime == 0x7fff || (unsigned)wTime == 0xffff)
    {
        /* ignore missing data */
        sprintf(szBuf, "n/a");
        return szBuf;
    }

    if(nHours > 12)
    {
            /* PM time ? */
            nHours -= 12;
            bAM = false;
    }
    else if(nHours == 12)
        bAM = false;

    if(nHours == 0 && bAM)
        nHours = 12;
    if(nMinutes)
        sprintf(szBuf, "%d:%02d%s", nHours, nMinutes, (bAM?"AM":"PM"));
    else
        sprintf(szBuf, "%d%s", nHours, (bAM?"AM":"PM"));


    return szBuf;
}




/**
 * Prints an encoded date value in the form '23-JUN-04'. If you don't like
 * this format, here is where you need to do your hacking.
 *
 * @param wDate Date integer to convert
 */
void PrintDate(uint16_t wDate)
{
    uint16_t w;

    if(wDate == 0xffff || wDate == 0) { /* ignore missing data */
        printf("n/a");
        return;
    }
    w = (wDate & 0x0f00) >> 7;          /* get msb of date */
    if(wDate & 0x0080)
        ++w;                            /* do crappy Davis format incr */
    printf("%02d-", w);

    w = (wDate & 0xf000) >> 12;         /* strip out month */
    switch(w) {
        case 1: printf("JAN"); break;
        case 2: printf("FEB"); break;
        case 3: printf("MAR"); break;
        case 4: printf("APR"); break;
        case 5: printf("MAY"); break;
        case 6: printf("JUN"); break;
        case 7: printf("JUL"); break;
        case 8: printf("AUG"); break;
        case 9: printf("SEP"); break;
        case 10: printf("OCT"); break;
        case 11: printf("NOV"); break;
        case 12: printf("DEC"); break;
        default: printf("???"); break;
    }

    printf("-20%02d", (wDate & 0x003f) );
}



/**
 * Prints time references for use in graph axis generations.
 */
void PrintTimeRef(void)
{
    struct tm stm;
    time_t tt;
    int i;

    /* 10min reference */
    printf("%s = ", _TIME_REF_10MIN );
    for(i = 23; i > -1; i--)
    {
        time(&tt);
        stm = *localtime(&tt);              /* get time now */
        stm.tm_min = (stm.tm_min/10)*10;    /* round to nearest 10min incr */
        stm.tm_min -= (10 * i);             /* back by 10 minutes */
        tt = mktime(&stm);
        stm = *localtime(&tt);              /* get time again */
        printf("%s", TimeConvert( (stm.tm_hour * 100)+stm.tm_min) );
        if(i)
            printf(",");
    }
    printf("\n");


    /* 15min reference */
    printf("%s = ", _TIME_REF_15MIN );
    for(i = 23; i > -1; i--)
    {
        time(&tt);
        stm = *localtime(&tt);              /* get time now */
        stm.tm_min = (stm.tm_min/15)*15;    /* round to nearest 15min incr */
        stm.tm_min -= (15 * i);             /* back by 15 minutes */
        tt = mktime(&stm);
        stm = *localtime(&tt);              /* get time again */
        printf("%s", TimeConvert( (stm.tm_hour * 100)+stm.tm_min) );
        if(i)
            printf(",");
    }
    printf("\n");




    /* hours reference */
    printf("%s = ", _TIME_REF_HOURS );
    for(i = 23; i > -1; i--)
    {
        time(&tt);
        stm = *localtime(&tt);          /* get time now */
        stm.tm_hour -= i;               /* back by hours */
        tt = mktime(&stm);
        stm = *localtime(&tt);          /* get time again */
        printf("%s", TimeConvert(stm.tm_hour * 100));
        if(i)
            printf(",");
    }
    printf("\n");


    /* days reference */
    printf("%s = ", _TIME_REF_DAYS);
    for(i = 24; i; i--)
    {
        time(&tt);
        stm = *localtime(&tt);          /* get time now */
        stm.tm_mday -= i;               /* back by days */
        tt = mktime(&stm);
        stm = *localtime(&tt);          /* get time again */
        printf("%d/%d", stm.tm_mon+1, stm.tm_mday);
        if(i > 1)
            printf(",");
    }
    printf("\n");

    /* months reference */
    printf("%s = ", _TIME_REF_MONTHS);
    time(&tt);
    stm = *localtime(&tt);              /* get time now */
    for(i = 24; i; i--)
    {
        switch(stm.tm_mon)
        {
            case 0: printf("JAN"); break;
            case 1: printf("FEB"); break;
            case 2: printf("MAR"); break;
            case 3: printf("APR"); break;
            case 4: printf("MAY"); break;
            case 5: printf("JUN"); break;
            case 6: printf("JUL"); break;
            case 7: printf("AUG"); break;
            case 8: printf("SEP"); break;
            case 9: printf("OCT"); break;
            case 10: printf("NOV"); break;
            case 11: printf("DEC"); break;
            default: printf("???"); break;
        }
        stm.tm_mon++;
        if (stm.tm_mon > 11) stm.tm_mon = 0;

        if(i > 1)
            printf(",");
    }
    printf("\n");

    /* years reference */
    printf("%s = ", _TIME_REF_YEARS);
    for(i = 25; i; i--)
    {
        time(&tt);
        stm = *localtime(&tt);          /* get time now */
        stm.tm_year -= i;               /* back by years */
        tt = mktime(&stm);
        stm = *localtime(&tt);          /* get time again */
        printf("%04d", stm.tm_year + 1900);
        if(i > 1)
            printf(",");
    }
    printf("\n");

}


/**
 * Dumps the highs/lows weather data to stdout.
 */
void PrintHLData(void)
{
    printf("%s = %2.2f\n", _BARO_LO_DAY, hld.wBaroLoDay / 1000.0 );
    printf("%s = %2.2f\n", _BARO_HI_DAY, hld.wBaroHiDay / 1000.0 );
    printf("%s = %2.2f\n", _BARO_LO_MONTH, hld.wBaroLoMonth / 1000.0 );
    printf("%s = %2.2f\n", _BARO_HI_MONTH, hld.wBaroHiMonth / 1000.0 );
    printf("%s = %2.2f\n", _BARO_LO_YEAR, hld.wBaroLoYear / 1000.0 );
    printf("%s = %2.2f\n", _BARO_HI_YEAR, hld.wBaroHiYear / 1000.0 );
    printf("%s = %s\n", _BARO_LO_TIME, TimeConvert(hld.wBaroLoTime));
    printf("%s = %s\n", _BARO_HI_TIME, TimeConvert(hld.wBaroHiTime));

    printf("%s = %d\n", _WIND_HI_DAY, hld.yWindHiDay );
    printf("%s = %s\n", _WIND_HI_TIME ,TimeConvert(hld.wWindHiTime));
    printf("%s = %d\n", _WIND_HI_MONTH, hld.yWindHiMonth );
    printf("%s = %d\n", _WIND_HI_YEAR, hld.yWindHiYear );

    printf("%s = %.1f\n", _IN_TEMP_HI_DAY, hld.wInTempHiDay / 10.0 );
    printf("%s = %.1f\n", _IN_TEMP_LO_DAY, hld.wInTempLoDay / 10.0 );
    printf("%s = %s\n", _IN_TEMP_HI_TIME, TimeConvert(hld.wInTempHiTime));
    printf("%s = %s\n", _IN_TEMP_LO_TIME, TimeConvert(hld.wInTempLoTime));
    printf("%s = %.1f\n", _IN_TEMP_LO_MONTH, hld.wInTempLoMonth / 10.0 );
    printf("%s = %.1f\n", _IN_TEMP_HI_MONTH, hld.wInTempHiMonth / 10.0 );
    printf("%s = %.1f\n", _IN_TEMP_LO_YEAR, hld.wInTempLoYear / 10.0 );
    printf("%s = %.1f\n", _IN_TEMP_HI_YEAR, hld.wInTempHiYear / 10.0 );

    printf("%s = %d\n", _IN_HUM_HI_DAY, hld.yInHumHiDay );
    printf("%s = %d\n", _IN_HUM_LO_DAY, hld.yInHumLoDay );
    printf("%s = %s\n", _IN_HUM_HI_TIME, TimeConvert(hld.wInHumHiTime));
    printf("%s = %s\n", _IN_HUM_LO_TIME, TimeConvert(hld.wInHumLoTime));
    printf("%s = %d\n", _IN_HUM_HI_MONTH, hld.yInHumHiMonth );
    printf("%s = %d\n", _IN_HUM_LO_MONTH, hld.yInHumLoMonth );
    printf("%s = %d\n", _IN_HUM_HI_YEAR, hld.yInHumHiYear );
    printf("%s = %d\n", _IN_HUM_LO_YEAR, hld.yInHumLoYear );

    printf("%s = %.1f\n", _TEMP_HI_DAY, hld.wTempHiDay / 10.0 );
    printf("%s = %.1f\n", _TEMP_LO_DAY, hld.wTempLoDay / 10.0 );
    printf("%s = %s\n", _TEMP_HI_TIME, TimeConvert(hld.wTempHiTime));
    printf("%s = %s\n", _TEMP_LO_TIME, TimeConvert(hld.wTempLoTime));
    printf("%s = %.1f\n", _TEMP_LO_MONTH, hld.wTempLoMonth / 10.0 );
    printf("%s = %.1f\n", _TEMP_HI_MONTH, hld.wTempHiMonth / 10.0 );
    printf("%s = %.1f\n", _TEMP_HI_YEAR, hld.wTempHiYear / 10.0 );
    printf("%s = %.1f\n", _TEMP_LO_YEAR, hld.wTempLoYear / 10.0 );

    printf("%s = %d\n", _DEW_LO_DAY, hld.wDewLoDay);
    printf("%s = %d\n", _DEW_HI_DAY, hld.wDewHiDay);
    printf("%s = %s\n", _DEW_LO_TIME, TimeConvert(hld.wDewLoTime));
    printf("%s = %s\n", _DEW_HI_TIME, TimeConvert(hld.wDewHiTime));
    printf("%s = %d\n", _DEW_HI_MONTH, hld.wDewHiMonth);
    printf("%s = %d\n", _DEW_LO_MONTH, hld.wDewLoMonth);
    printf("%s = %d\n", _DEW_HI_YEAR, hld.wDewHiYear);
    printf("%s = %d\n", _DEW_LO_YEAR, hld.wDewLoYear);

    printf("%s = %d\n", _CHILL_LO_DAY, hld.wChillLoDay);
    printf("%s = %s\n", _CHILL_LO_TIME, TimeConvert(hld.wChillLoTime));
    printf("%s = %d\n", _CHILL_LO_MONTH, hld.wChillLoMonth);
    printf("%s = %d\n", _CHILL_LO_YEAR, hld.wChillLoYear);

    printf("%s = %d\n", _HEAT_HI_DAY, hld.wHeatHiDay);
    printf("%s = %s\n", _HEAT_HI_TIME, TimeConvert(hld.wHeatHiTime));
    printf("%s = %d\n", _HEAT_HI_MONTH, hld.wHeatHiMonth);
    printf("%s = %d\n", _HEAT_HI_YEAR, hld.wHeatHiYear);

    printf("%s = %.1f\n", _SOLAR_HI_DAY, hld.wSolarHiDay / 10.0 );
    printf("%s = ", _SOLAR_HI_TIME );
    if(hld.wSolarHiTime == 65535)
        printf("n/a\n");
    else
        printf("%s\n", TimeConvert(hld.wSolarHiTime));
    printf("%s = %.1f\n", _SOLAR_HI_MONTH, hld.wSolarHiMonth / 10.0 );
    printf("%s = %.1f\n", _SOLAR_HI_YEAR, hld.wSolarHiYear / 10.0 );

    printf("%s = %.1f\n", _UV_HI_DAY, hld.yUVHiDay / 10.0);
    printf("%s = ", _UV_HI_TIME );
    if(hld.wUVHiTime == 65535)
        printf("n/a\n");
    else
        printf("%s\n", TimeConvert(hld.wUVHiTime));
    printf("%s = %.1f\n", _UV_HI_MONTH, hld.yUVHiMonth / 10.0);
    printf("%s = %.1f\n", _UV_HI_YEAR, hld.yUVHiYear / 10.0);

    printf("%s = %.2f\n", _RAIN_RATE_HI_DAY, hld.wRainHiDay / 100.0);
    printf("%s = ", _RAIN_RATE_HI_TIME );
    if(hld.wRainHiTime == 65535)
        printf("n/a\n");
    else
    printf("%s\n", TimeConvert(hld.wRainHiTime));
    printf("%s = %.2f\n", _RAIN_RATE_HI_HOUR, hld.wRainHiHour / 100.0);
    printf("%s = %.2f\n", _RAIN_RATE_HI_MONTH, hld.wRainHiMonth / 100.0);
    printf("%s = %.2f\n", _RAIN_RATE_HI_YEAR, hld.wRainHiYear / 100.0);

}


/**
 * Dumps the graphics sets to stdout.
 *
 * @param pData Pointer to string containing the data print print
 */
void PrintGDData(uint8_t * pData)
{
    PrintTimeRef();                 /* Print reference times */

    /* inside temp by hour, last 24 */
    printf("%s = ", _IN_TEMP_BY_HOURS);
    PrintTempSet24(pData, TEMP_IN_HOUR, *(pData + NEXT_HOUR_PTR), 90);

    /* inside temp highs by day, last 24 */
    printf("%s = ", _IN_TEMP_HI_BY_DAYS);
    PrintTempSet24(pData, TEMP_IN_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 90);

    /* inside temp highs by day, time, last 24 */
    printf("%s = ", _IN_TEMP_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, TEMP_IN_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* inside temp lows by day, last 24 */
    printf("%s = ", _IN_TEMP_LO_BY_DAYS);
    PrintTempSet24(pData, TEMP_IN_DAY_LOWS, *(pData + NEXT_DAY_PTR), 90);

    /* inside temp lows by day by time, last 24 */
    printf("%s = ", _IN_TEMP_LO_BY_DAYS_TIME);
    PrintTimeSet(pData, TEMP_IN_DAY_LOW_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* inside temp highs by month, last 25 */
    printf("%s = ", _IN_TEMP_HI_BY_MONTHS);
    PrintTempSet25(pData, TEMP_IN_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 90);

    /* inside temp lows by month, last 25 */
    printf("%s = ", _IN_TEMP_LO_BY_MONTHS);
    PrintTempSet25(pData, TEMP_IN_MONTH_LOWS, *(pData + NEXT_MONTH_PTR), 90);

    /* temp by hour, last 24 */
    printf("%s = ", _TEMP_BY_HOURS);
    PrintTempSet24(pData, TEMP_OUT_HOUR, *(pData + NEXT_HOUR_PTR),  90);

    /* temp highs by day, last 24 */
    printf("%s = ", _TEMP_HI_BY_DAYS);
    PrintTempSet24(pData, TEMP_OUT_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 90);

    /* temp highs by day, time, last 24 */
    printf("%s = ", _TEMP_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, TEMP_OUT_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* temp lows by day, last 24 */
    printf("%s = ", _TEMP_LO_BY_DAYS);
    PrintTempSet24(pData, TEMP_OUT_DAY_LOWS, *(pData + NEXT_DAY_PTR), 90);

    /* temp lows by day by time, last 24 */
    printf("%s = ", _TEMP_LO_BY_DAYS_TIME);
    PrintTimeSet(pData, TEMP_OUT_DAY_LOW_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* temp highs by month, last 24 */
    printf("%s = ", _TEMP_HI_BY_MONTHS);
    PrintTempSet25(pData, TEMP_OUT_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 90);

    /* temp lows by month, last 24 */
    printf("%s = ", _TEMP_LO_BY_MONTHS);
    PrintTempSet25(pData, TEMP_OUT_MONTH_LOWS, *(pData + NEXT_MONTH_PTR), 90);

    /* dew point by hour, last 24 */
    printf("%s = ", _DEW_BY_HOURS);
    PrintTempSet24(pData, DEW_HOUR, *(pData + NEXT_HOUR_PTR), 120);

    /* dew point highs by day, last 24 */
    printf("%s = ", _DEW_HI_BY_DAYS);
    PrintTempSet24(pData, DEW_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 120);

    /* dew point highs by day, time, last 24 */
    printf("%s = ", _DEW_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, DEW_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* dew point lows by day, last 24 */
    printf("%s = ", _DEW_LO_BY_DAYS);
    PrintTempSet24(pData, DEW_DAY_LOWS, *(pData + NEXT_DAY_PTR), 120);

    /* dew point lows by day, time, last 24 */
    printf("%s = ", _DEW_LO_BY_DAYS_TIME);
    PrintTimeSet(pData, DEW_DAY_LOW_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* dew point highs by month, last 25 */
    printf("%s = ", _DEW_HI_BY_MONTHS);
    PrintTempSet25(pData, DEW_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 120);

    /* dew point lows by month, last 25 */
    printf("%s = ",  _DEW_LO_BY_MONTHS);
    PrintTempSet25(pData, DEW_MONTH_LOWS, *(pData + NEXT_MONTH_PTR), 120);

    /* wind chill by hour, last 24 */
    printf("%s = ", _CHILL_LO_BY_HOURS);
    PrintTempSet24(pData, CHILL_HOUR, *(pData + NEXT_HOUR_PTR), 120);

    /* wind chill lows by day, last 24 */
    printf("%s = ", _CHILL_LO_BY_DAYS);
    PrintTempSet24(pData, CHILL_DAY_LOWS, *(pData + NEXT_DAY_PTR), 120);

    /* wind chill lows by day, time, last 24 */
    printf("%s = ", _CHILL_LO_BY_DAYS_TIME);
    PrintTimeSet(pData, CHILL_DAY_LOW_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* wind chill lows by month, last 25 */
    printf("%s = ", _CHILL_LO_BY_MONTHS);
    PrintTempSet25(pData, CHILL_MONTH_LOWS, *(pData + NEXT_MONTH_PTR), 120);

    /* heat index by hour, last 24 */
    printf("%s = ", _HEAT_HI_BY_HOURS);
    PrintTempSet24(pData, HEAT_HOUR, *(pData + NEXT_HOUR_PTR), 90);

    /* heat index lows by day, last 24 */
    printf("%s = ", _HEAT_HI_BY_DAYS);
    PrintTempSet24(pData, HEAT_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 90);

    /* heat index highs by day, time, last 24 */
    printf("%s = ", _HEAT_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, HEAT_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* heat index by month, last 25 */
    printf("%s = ", _HEAT_HI_BY_MONTHS);
    PrintTempSet25(pData, HEAT_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 90);

    /* Inside Humidity by hour, last 24 */
    printf("%s = ", _IN_HUM_BY_HOURS);
    PrintByteSet(pData, HUM_IN_HOUR, *(pData + NEXT_HOUR_PTR), 24);

    /* Inside Humidity highs by day, last 24 */
    printf("%s = ", _IN_HUM_HI_BY_DAYS);
    PrintByteSet(pData, HUM_IN_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 24);

    /* Inside Humidity highs by day, time, last 24 */
    printf("%s = ", _IN_HUM_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, HUM_IN_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* Inside Humidity lows by day, last 24 */
    printf("%s = ", _IN_HUM_LO_BY_DAYS);
    PrintByteSet(pData, HUM_IN_DAY_LOWS, *(pData + NEXT_DAY_PTR), 24);

    /* Inside Humidity lows by day, time, last 24 */
    printf("%s = ", _IN_HUM_LO_BY_DAYS_TIME);
    PrintTimeSet(pData, HUM_IN_DAY_LOW_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* Inside Humidity highs by month, last 24 */
    printf("%s = ", _IN_HUM_HI_BY_MONTHS);
    PrintByteSet(pData, HUM_IN_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 25);

    /* Inside Humidity lows by month, last 24 */
    printf("%s = ", _IN_HUM_LO_BY_MONTHS);
    PrintByteSet(pData, HUM_IN_MONTH_LOWS, *(pData + NEXT_MONTH_PTR), 25);

    /* Humidity by hour, last 24 */
    printf("%s = ", _HUM_BY_HOURS);
    PrintByteSet(pData, HUM_OUT_HOUR, *(pData + NEXT_HOUR_PTR), 24);

    /* Humidity highs by day, last 24 */
    printf("%s = ", _HUM_HI_BY_DAYS);
    PrintByteSet(pData, HUM_OUT_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 24);

    /* Humidity highs by day, time, last 24 */
    printf("%s = ", _HUM_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, HUM_OUT_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* Humidity lows by day, last 24 */
    printf("%s = ", _HUM_LO_BY_DAYS);
    PrintTimeSet(pData, HUM_OUT_DAY_LOWS, *(pData + NEXT_DAY_PTR), 24);

    /* Humidity lows by day, time, last 24 */
    printf("%s = ", _HUM_LO_BY_DAYS_TIME);
    PrintTimeSet(pData, HUM_OUT_DAY_LOW_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* Humidity highs by month, last 24 */
    printf("%s = ", _HUM_HI_BY_MONTHS);
    PrintByteSet(pData, HUM_OUT_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 25);

    /* Humidity lows by month, last 24 */
    printf("%s = ", _HUM_LO_BY_MONTHS);
    PrintByteSet(pData, HUM_OUT_MONTH_LOWS, *(pData + NEXT_MONTH_PTR), 25);

    /* Barometer 15 min increments, last 24 */
    printf("%s = ", _BAR_BY_15MIN);
    PrintBarSet(pData, BAR_15_MIN, *(pData + NEXT_15MIN_PTR), 24);

    /* Barometer by hour, last 24 */
    printf("%s = ", _BAR_BY_HOURS);
    PrintBarSet(pData, BAR_HOUR, *(pData + NEXT_HOUR_PTR), 24);

    /* Barometer highs by day, last 24 */
    printf("%s = ", _BAR_HI_BY_DAYS);
    PrintBarSet(pData, BAR_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 24);

    /* Barometer highs by day, time, last 24 */
    printf("%s = ", _BAR_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, BAR_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* Barometer lows by day, last 24 */
    printf("%s = ", _BAR_LO_BY_DAYS);
    PrintBarSet(pData, BAR_DAY_LOWS, *(pData + NEXT_DAY_PTR), 24);

    /* Barometer lows by day by time, last 24 */
    printf("%s = ", _BAR_LO_BY_DAYS_TIME);
    PrintTimeSet(pData, BAR_DAY_LOW_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* Barometer highs by month, last 25 */
    printf("%s = ", _BAR_HI_BY_MONTHS);
    PrintBarSet(pData, BAR_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 25);

    /* Barometer lows by month, last 25 */
    printf("%s = ", _BAR_LO_BY_MONTHS);
    PrintBarSet(pData, BAR_MONTH_LOWS, *(pData + NEXT_MONTH_PTR), 25);

    /* Wind speed 10 min averages, last 24 */
    printf("%s = ", _WIND_AVG_BY_10MIN);
    PrintByteSet(pData, WIND_SPEED_10_MIN_AVG, *(pData + NEXT_10MIN_PTR), 24);

    /* Wind speed average by hour, last 24 */
    printf("%s = ", _WIND_AVG_BY_HOURS);
    PrintByteSet(pData, WIND_SPEED_HOUR_AVG, *(pData + NEXT_HOUR_PTR), 24);

    /* Wind speed highs by day, last 24 */
    printf("%s = ", _WIND_HI_BY_DAYS);
    PrintByteSet(pData, WIND_SPEED_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 24);

    /* Wind speed highs by day, time, last 24 */
    printf("%s = ", _WIND_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, WIND_SPEED_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* Wind speed highs by month, last 25 */
    printf("%s = ", _WIND_HI_BY_MONTHS);
    PrintByteSet(pData, WIND_SPEED_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 25);

    /* Wind speed highs by year, last 25 */
    printf("%s = ", _WIND_HI_BY_YEARS);
    PrintByteSet(pData, WIND_SPEED_YEAR_HIGHS, *(pData + NEXT_YEAR_PTR), 25);

    /* Wind direction by hour, last 24 */
    printf("%s = ", _WIND_DIR_BY_HOURS);
    PrintByteSet(pData, WIND_DIR_HOUR, *(pData + NEXT_HOUR_PTR), 24);

    /* Wind direction by day, last 24 */
    printf("%s = ", _WIND_DIR_BY_DAYS);
    PrintByteSet(pData, WIND_DIR_DAY, *(pData + NEXT_DAY_PTR), 24);

    /* Wind direction by month, last 25 */
    printf("%s = ", _WIND_DIR_BY_MONTHS);
    PrintByteSet(pData, WIND_DIR_MONTH, *(pData + NEXT_MONTH_PTR), 25);

    /* Rain Rate by minute, last 24 */
    printf("%s = ", _RAIN_RATE_BY_MINUTES);
    PrintRainRateSet(pData, RAIN_RATE_1_MIN, *(pData + NEXT_10MIN_PTR), 24);

    /* Rain Rate by hour, last 24 */
    printf("%s = ", _RAIN_RATE_BY_HOURS);
    PrintRainRateSet(pData, RAIN_RATE_HOUR, *(pData + NEXT_HOUR_PTR), 24);

    /* Rain Rate highs by day, last 24 */
    printf("%s = ", _RAIN_RATE_HI_BY_DAYS);
    PrintRainRateSet(pData, RAIN_RATE_DAY_HIGHS, *(pData + NEXT_DAY_PTR), 24);

    /* Rain Rate highs by day time, last 24 */
    printf("%s = ", _RAIN_RATE_HI_BY_DAYS_TIME);
    PrintTimeSet(pData, RAIN_RATE_DAY_HIGH_TIMES, *(pData + NEXT_DAY_PTR), 24);

    /* Rain Rate highs by month, last 25 */
    printf("%s = ", _RAIN_RATE_HI_BY_MONTHS);
    PrintRainRateSet(pData, RAIN_RATE_MONTH_HIGHS, *(pData + NEXT_MONTH_PTR), 25);

    /* Rain Rate highs by years, last 25 */
    printf("%s = ", _RAIN_RATE_HI_BY_YEARS);
    PrintRainRateSet(pData, RAIN_RATE_YEAR_HIGHS, *(pData + NEXT_YEAR_PTR), 25);

    /* Rain by hour, last 24 */
    printf("%s = ", _RAIN_BY_HOUR);
    PrintRainRateSet(pData, RAIN_HOUR, *(pData + NEXT_HOUR_PTR), 24);

    /* Rain by storm, last 25 */
    printf("%s = ", _RAIN_STORMS);
    PrintRainRateSet(pData, RAIN_STORM, *(pData + NEXT_RAIN_STORM_PTR), 25);

    /* Rain by storm start date, last 25 */
    printf("%s = ", _RAIN_STORMS_START_DATE);
    PrintDateSet(pData, RAIN_STORM_START, *(pData + NEXT_RAIN_STORM_PTR), 25);

    /* Rain by storm end date, last 25 */
    printf("%s = ", _RAIN_STORMS_END_DATE);
    PrintDateSet(pData, RAIN_STORM_END, *(pData + NEXT_RAIN_STORM_PTR), 25);

    /* Rain by day, last 25 */
    printf("%s = ", _RAIN_BY_DAY);
    PrintRainRateSet(pData, RAIN_DAY_TOTAL, *(pData + NEXT_DAY_PTR), 25);

    /* Rain by month, last 25 */
    printf("%s = ", _RAIN_BY_MONTH);
    PrintRainRateSet(pData, RAIN_MONTH_TOTAL, *(pData + NEXT_MONTH_PTR), 25);

    /* Rain by year, last 25 */
    printf("%s = ", _RAIN_BY_YEAR);
    PrintRainRateSet(pData, RAIN_YEAR_TOTAL, *(pData + NEXT_RAIN_YEAR_PTR), 25);

}




/**
 * Prints a comma separated list of temperature values.
 *
 * @param pData Pointer to string containing data items
 * @param nOffset Offset of first data item
 * @param yNext Number of bytes to move for next data item
 * @param nSub Offset value to subtract off value
 */
void PrintTempSet24(uint8_t *pData, int nOffset, uint8_t yNext, int nSub)
{
    int i, nEntry;
    int16_t sValue;

    nEntry = nOffset + yNext;

    for(i = 0; i < 24; i++) {           /* get data set in proper order */
        sValue = *(pData + nEntry);
        if(sValue != 255) {                 /* force n/a's to empty values */
            printf("%d", sValue - nSub);
        }
        if(++nEntry >= (nOffset + 24))
            nEntry = nOffset;               /* reset to beginning */
        if(i < 23)
            printf(",");                    /* no comma on last entry */
    }
    printf("\n");

}



/**
 * Prints a comma separated list of temperature values from a 25 data set.
 * The Davis EEPROM keeps 24+1 records in some data sets, which the extra one
 * is the current value (month or year). We discard this to keep all graphs
 * consistent at 24. In all cases, you can get this data from the highs/lows
 * data set anyway.
 *
 * @param pData Pointer to string containing data items
 * @param nOffset Offset of first data item
 * @param yNext Number of bytes to move for next data item
 * @param nSub Offset value to subtract off value
 */
void PrintTempSet25(uint8_t *pData, int nOffset, uint8_t yNext, int nSub)
{
    int i, nEntry;
    int16_t sValue;

    nEntry = nOffset + yNext;

    for(i = 0; i < 25; i++) {           /* get data set in proper order */
        sValue = *(pData + nEntry);
        if(++nEntry >= (nOffset + 25))
            nEntry = nOffset;               /* reset to beginning */
        else {
            if(sValue != 255) {             /* force n/a's to be empty values */
              /* subtract nSub to get temp integer */
              printf("%d", sValue - nSub);
            }
            if(i < 24)
                printf(",");                /* no comma on last entry */
        }
    }
    printf("\n");

}


/**
 * Prints a comma separated list of byte values.
 *
 * @param pData Pointer to string containing data items
 * @param nOffset Offset of first data item
 * @param yNext Number of bytes to move for next data item
 * @param nSetSize Number of items in data set
 */
void PrintByteSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize)
{
    int i, nEntry;
    uint8_t yValue;

    nEntry = nOffset + yNext;

    for(i = 0; i < nSetSize; i++) {         /* get data set in proper order */
        yValue = *(pData + nEntry);
            yValue = 0;
        if(++nEntry >= (nOffset + nSetSize)) {
            nEntry = nOffset;               /* reset to beginning */
            if(nSetSize != 25) {
                if(yValue != 255) {         /* force n/a's to be empty values */
                    printf("%d", yValue);
                }
                if(i < nSetSize - 1) printf(",");
            }
        }
        else {
            printf("%d", yValue);
            if(i < nSetSize - 1)
                printf(",");                    /* no comma on last entry */
        }
    }
    printf("\n");

}



/**
 * Prints the comma separated list of times.
 *
 * @param pData Pointer to string containing data items
 * @param nOffset Offset of first data item
 * @param yNext Number of bytes to move for next data item
 * @param nSetSize Number of items in data set
 */
void PrintTimeSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize)
{
    int i, nEntry;
    uint16_t wValue;

    nEntry = nOffset + (yNext * 2);

    for(i = 0; i < nSetSize; i++) {         /* get data set in proper order */
        wValue = *( (uint16_t*)(pData + nEntry) );
        if(wValue != 65535) {               /* print blank for empty entry */
            printf("%s", TimeConvert(wValue));
        }
        nEntry += 2;
        if(nEntry >= nOffset + (nSetSize*2))
            nEntry = nOffset;               /* reset to beginning */
        if(i < nSetSize - 1)
            printf(",");                    /* no comma on last entry */
    }
    printf("\n");
}



/**
 * Prints a comma separated list of barometers.
 *
 * @param pData Pointer to string containing data items
 * @param nOffset Offset of first data item
 * @param yNext Number of bytes to move for next data item
 * @param nSetSize Number of items in data set
 */
void PrintBarSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize)
{
    int i, nEntry;
    uint16_t wValue;

    nEntry = nOffset + (yNext * 2);

    for(i = 0; i < nSetSize; i++) {         /* get data set in proper order */
        wValue = *( (uint16_t*)(pData + nEntry) );
        nEntry += 2;
        if(nEntry >= nOffset + (nSetSize*2)) {
            nEntry = nOffset;               /* reset to beginning */
            if(nSetSize != 25) {
                if(wValue == 65535) {       /* print empty value for illegal entries */
                    printf("%2.2f", wValue / 1000.0);
                }
                if(i < nSetSize - 1) printf(",");
            }

        }
        else {
            printf("%2.2f", wValue / 1000.0);
            if(i < nSetSize - 1)
                printf(",");                    /* no comma on last entry */
        }
    }
    printf("\n");
}





/**
 * Prints a comma separated list of rain rates.
 *
 * @param pData Pointer to string containing data items
 * @param nOffset Offset of first data item
 * @param yNext Number of bytes to move for next data item
 * @param nSetSize Number of items in data set
 */
void PrintRainRateSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize)
{
    int i, nEntry;
    uint16_t wValue;

    nEntry = nOffset + (yNext * 2);

    for(i = 0; i < nSetSize; i++) {         /* get data set in proper order */
        wValue = *( (uint16_t*)(pData + nEntry) );
        nEntry += 2;
        if(nEntry >= nOffset + (nSetSize*2)) {
            nEntry = nOffset;               /* reset to beginning */
            if(nSetSize != 25) {
                if(wValue == 65535) {     /* print empty value for illegal entries */
                    printf("%.2f", wValue / 100.0);
                }
                if(i < nSetSize - 1) printf(",");
            }
        }
        else {
            printf("%.2f", wValue / 100.0);
            if(i < nSetSize - 1)
                printf(",");                    /* no comma on last entry */
        }
    }
    printf("\n");
}




/**
 * Prints a comma separated list of date items.
 *
 * @param pData Pointer to string containing data items
 * @param nOffset Offset of first data item
 * @param yNext Number of bytes to move for next data item
 * @param nSetSize Number of items in data set
 */
void PrintDateSet(uint8_t *pData, int nOffset, uint8_t yNext, int nSetSize)
{
    int i, nEntry;
    uint16_t wValue;

    nEntry = nOffset + (yNext * 2);

    for(i = 0; i < nSetSize; i++) {         /* get data set in proper order */
        wValue = *( (uint16_t*)(pData + nEntry) );
        nEntry += 2;
        if(nEntry >= nOffset + (nSetSize*2)) {
            nEntry = nOffset;               /* reset to beginning */
            if(nSetSize != 25) {
                PrintDate(wValue);
                if(i < nSetSize - 1) printf(",");
            }
        }
        else {
            PrintDate(wValue);
            if(i < nSetSize - 1)
                printf(",");                    /* no comma on last entry */
        }
    }
    printf("\n");
}


/**
 * Prints how many racords will be downloaded to stdout
 */
void PrintDownloadInfo(void)
{
  printf("%d pages of records will be sent\n", arcInfo.numberOfPages);
}


/**
 * Returns the number of pages that will be sent by the archive download
 * command (from the details received from the unit)
 *
 * @returns Number of pages to be downloaded
 */
uint16_t GetNumberOfPages(void)
{
  return arcInfo.numberOfPages;
}



/**
 * Prints the header for the archive records to stdout
 */
void PrintArchHeader(void)
{
  printf("date,time,");
  // Temperatures
  printf("outside temp (°F),outside temp high (°F), outside temp low (°F),insideTemp (°F),");
  printf("extra temp1 (°F), extra temp2 (°F), extra temp3 (°F),");
  // Humidity
  printf("inside humidity (%%),outside humidity (%%),");
  printf("extra humidity 1 (%%),extra humidity 2 (%%),");
  // Rainfall
  printf("rainfall (clicks),highest rain rate (clicks/hr),");
  // Barometer and forecast
  printf("barometer (inches Hg),forecast at end of period,");
  // Wind speed
  printf("number of wind samples,");
  printf("average wind speed (mph),prevailing wind direction (°),prevailing wind rose,");
  printf("highest wind speed (mph),highest wind direction (°),higest wind rose,");
  // Radiation
  printf("average solar radiation (W/m^2),maximum solar radiation (W/m^2),");
  printf("average uv index,max uv,accumulated et (in),");
  // Leaf
  printf("leaf temp 1 (°F),leaf temp 2 (°F),leaf wetness 1,leaf wetness 2,");
  // Soil
  printf("soil temp 1 (°F),soil temp 2 (°F),soil temp 3 (°F),soil temp 4 (°F),");
  printf("soil moisture 1 (cb),soil moisture 2 (cb), soil moisture 3 (cb), soil moisture 4 (cb)\n");
}



/**
 *  Dumps the archivepacket records to stdout.
 *
 *  @param maxArchRecords Maximum number of archive records to print
 *      (0 to print all)
 */
void PrintArchPacket(int maxArcRecords)
{
  int i;
  time_t currentTime;
  struct tm *timeStruct;
  uint16_t currentDate;
  ARCDATAB *record;

  // Get Current date
  time(&currentTime);
  timeStruct = localtime(&currentTime);
  currentDate = MakeVangateDateInt(timeStruct);

  if (maxArcRecords <= 0 || maxArcRecords > 5) {
    maxArcRecords = 5;
  }

  for (i = 0; i < maxArcRecords; i++) {
    record = &arcPacket.records[i];

    if (record->date > currentDate) {
      continue;
    }

    // Date and time
    printf("%04d-%02d-%02d,", DATESTAMP_YEAR(record->date),
          DATESTAMP_MONTH(record->date), DATESTAMP_DAY(record->date));
    printf("%02d:%02d,", TIMESTAMP_HOUR(record->time), TIMESTAMP_MINUTE(record->time));

    // Outside/Inside temperatures
    PRINTDECIMAL(TEMP(record->outsideTemp), 32767, true);
    PRINTDECIMAL(TEMP(record->outsideHighTemp), -32767, true);
    PRINTDECIMAL(TEMP(record->outsideLowTemp), 32767, true);
    PRINTDECIMAL(TEMP(record->insideTemp), 32767, true);

    // Extra temperatures (** dash value incorrectly set to 255 on VP2)
    PRINTINT(record->extraTemps[0], 255, true);
    PRINTINT(record->extraTemps[1], 255, true);
    PRINTINT(record->extraTemps[2], 255, true);


    // Humidity
    PRINTINT(record->outsideHum, 255, true);
    PRINTINT(record->insideHum, 255, true);
    PRINTINT(record->extraHums[0], 255, true);
    PRINTINT(record->extraHums[1], 255, true);

    // Rainfall
    PRINTINT(record->rainfall, 0, true);
    PRINTINT(record->highestRainRate, 0, true);

    // Barometer and forecast
    PRINTTHOUSANDTHS(record->barometer / 1000.0, 0, true);
    printf("%s,", ForecastString(record->forecastRule));

    // Wind speed
    PRINTINT(record->windSamples, 0, true);
    PRINTINT(record->avgWindSpd, 255, true);
    PRINTINT(record->prevailWindDir, 32767, true);
    printf("%s,", getWindRose(record->prevailWindDir));
    PRINTINT(record->highestWindSpd, 0, true);
    PRINTINT(record->highestWindDir, 32767, true);
    printf("%s,", getWindRose(record->highestWindDir));

    // Radiation
    PRINTINT(record->avgSolarRad, 32767, true);
    PRINTINT(record->solarRadMax, 0, true);
    PRINTINT(record->avgUvIndex, 255, true);
    PRINTDECIMAL(record->uvMax / 10.0, 0, true);
    PRINTTHOUSANDTHS(record->etAccumulated / 1000.0, 0, true);

    // Leaf
    PRINTINT(ROUGH_TEMP(record->leafTemps[0]), 165, true);
    PRINTINT(ROUGH_TEMP(record->leafTemps[1]), 165, true);
    PRINTINT(record->leafWetness[0], 255, true);
    PRINTINT(record->leafWetness[1], 255, true);

    //Soil (soil temp dash values are 255-90)
    PRINTINT(ROUGH_TEMP(record->soilTemps[0]), 165, true);
    PRINTINT(ROUGH_TEMP(record->soilTemps[1]), 165, true);
    PRINTINT(ROUGH_TEMP(record->soilTemps[2]), 165, true);
    PRINTINT(ROUGH_TEMP(record->soilTemps[3]), 165, true);
    PRINTINT(record->soilMoisture[0], 255, true);
    PRINTINT(record->soilMoisture[1], 255, true);
    PRINTINT(record->soilMoisture[2], 255, true);
    PRINTINT(record->soilMoisture[3], 255, false);

    printf("\n");
  }
}

/**
 * Returns the forecast string for the given forecast rule. I used a brute
 * force approach here based on the research work done by www.oftedahl.no/.
 * The Davis forecast rule byte is actually a compound number where certain
 * bits or formulae can be applied to obtain sets of substrings. However, I
 * haven't been able to figure out the math behind this, and it remains
 * undocumented by Davis. Hence the brute force approach.
 *
 * @param wRule Integer index of forecast rule
 *
 * @returns Pointer to forecast rule string
 */
char* ForecastString(uint16_t wRule)
{
    char *ptr;

    if(wRule > 194)
        wRule = 194;                /* trap out of bounds */

    ptr = szForeStrings;
    while(wRule--) {                /* walk thru the null terminators */
        while(*ptr++)
            ;
    }


    return ptr;

}
