/****************************************************************************/
/*  vproweather 0.8                                                         */
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
/* dhandler.h - data handlers include file                                  */
/* tabs every 4 places                                                      */
/*                                                                          */
/****************************************************************************/

#ifndef __DHANDLER_H__
#define __DHANDLER_H__

#include "main.h"

#ifdef __cplusplus
#define extern "C" {        /* respect c++ callers */
#endif

/* exports */
extern int CheckCRC(int nCnt, char *pData);
extern void GetRTData(char *szData);
extern void GetHLData(char *szData);
extern void PrintRTData(void);
extern void PrintHLData(void);
extern void PrintGDData(uint8_t * pData);
extern void PrintTime(char *szData);

extern char* ForecastString(uint16_t wRule);


/* graphic data sets- memory pointer offsets */
#define GRAPH_START              177

#define NEXT_10MIN_PTR           GRAPH_START+1
#define NEXT_15MIN_PTR           GRAPH_START+2
#define NEXT_HOUR_PTR            GRAPH_START+3
#define NEXT_DAY_PTR             GRAPH_START+4
#define NEXT_MONTH_PTR           GRAPH_START+5
#define NEXT_YEAR_PTR            GRAPH_START+6
#define NEXT_RAIN_STORM_PTR      GRAPH_START+7
#define NEXT_RAIN_YEAR_PTR       GRAPH_START+8

#define GRF                      326
//                                                        NUMBER  NUMBER
//                                                        OF      OF
//                                                        ENTRYS  BYTES
//                                                        --------------
#define TEMP_IN_HOUR                 GRF +    0            // 24 ||  1
#define TEMP_IN_DAY_HIGHS            GRF +   24            // 24 ||  1
#define TEMP_IN_DAY_HIGH_TIMES       GRF +   48            // 24 ||  2
#define TEMP_IN_DAY_LOWS             GRF +   96            // 24 ||  1
#define TEMP_IN_DAY_LOW_TIMES        GRF +  120            // 24 ||  2
#define TEMP_IN_MONTH_HIGHS          GRF +  168            // 25 ||  1
#define TEMP_IN_MONTH_LOWS           GRF +  193            // 25 ||  1
#define TEMP_IN_YEAR_HIGHS           GRF +  218            //  1 ||  1
#define TEMP_IN_YEAR_LOWS            GRF +  219            //  1 ||  1

#define TEMP_OUT_HOUR                GRF +  220            // 24 ||  1
#define TEMP_OUT_DAY_HIGHS           GRF +  244            // 24 ||  1
#define TEMP_OUT_DAY_HIGH_TIMES      GRF +  268            // 24 ||  2
#define TEMP_OUT_DAY_LOWS            GRF +  316            // 24 ||  1
#define TEMP_OUT_DAY_LOW_TIMES       GRF +  340            // 24 ||  2
#define TEMP_OUT_MONTH_HIGHS         GRF +  388            // 25 ||  1
#define TEMP_OUT_MONTH_LOWS          GRF +  413            // 25 ||  1
#define TEMP_OUT_YEAR_HIGHS          GRF +  438            // 25 ||  1
#define TEMP_OUT_YEAR_LOWS           GRF +  463            // 25 ||  1

#define DEW_HOUR                     GRF +  488            // 24 ||  1
#define DEW_DAY_HIGHS                GRF +  512            // 24 ||  1
#define DEW_DAY_HIGH_TIMES           GRF +  536            // 24 ||  2
#define DEW_DAY_LOWS                 GRF +  584            // 24 ||  1
#define DEW_DAY_LOW_TIMES            GRF +  608            // 24 ||  2
#define DEW_MONTH_HIGHS              GRF +  656            // 25 ||  1
#define DEW_MONTH_LOWS               GRF +  681            // 25 ||  1
#define DEW_YEAR_HIGHS               GRF +  706            //  1 ||  1
#define DEW_YEAR_LOWS                GRF +  707            //  1 ||  1

#define CHILL_HOUR                   GRF +  708            // 24 ||  1
#define CHILL_DAY_LOWS               GRF +  732            // 24 ||  1
#define CHILL_DAY_LOW_TIMES          GRF +  756            // 24 ||  2
#define CHILL_MONTH_LOWS             GRF +  804            // 25 ||  1
#define CHILL_YEAR_LOWS              GRF +  829            //  1 ||  1

#define THSW_HOUR                    GRF +  830            // 24 ||  1
#define THSW_DAY_HIGHS               GRF +  854            // 24 ||  1
#define THSW_DAY_HIGH_TIMES          GRF +  878            // 24 ||  2
#define THSW_MONTH_HIGHS             GRF +  926            // 25 ||  1
#define THSW_YEAR_HIGHS              GRF +  951            //  1 ||  1

#define HEAT_HOUR                    GRF +  952            // 24 ||  1
#define HEAT_DAY_HIGHS               GRF +  976            // 24 ||  1
#define HEAT_DAY_HIGH_TIMES          GRF + 1000            // 24 ||  2
#define HEAT_MONTH_HIGHS             GRF + 1048            // 25 ||  1
#define HEAT_YEAR_HIGHS              GRF + 1073            //  1 ||  1

#define HUM_IN_HOUR                  GRF + 1074            // 24 ||  1
#define HUM_IN_DAY_HIGHS             GRF + 1098            // 24 ||  1
#define HUM_IN_DAY_HIGH_TIMES        GRF + 1122            // 24 ||  2
#define HUM_IN_DAY_LOWS              GRF + 1170            // 24 ||  1
#define HUM_IN_DAY_LOW_TIMES         GRF + 1194            // 24 ||  2
#define HUM_IN_MONTH_HIGHS           GRF + 1242            // 25 ||  1
#define HUM_IN_MONTH_LOWS            GRF + 1267            // 25 ||  1
#define HUM_IN_YEAR_HIGHS            GRF + 1292            //  1 ||  1
#define HUM_IN_YEAR_LOWS             GRF + 1293            //  1 ||  1

#define HUM_OUT_HOUR                 GRF + 1294            // 24 ||  1
#define HUM_OUT_DAY_HIGHS            GRF + 1318            // 24 ||  1
#define HUM_OUT_DAY_HIGH_TIMES       GRF + 1342            // 24 ||  2
#define HUM_OUT_DAY_LOWS             GRF + 1390            // 24 ||  1
#define HUM_OUT_DAY_LOW_TIMES        GRF + 1414            // 24 ||  2
#define HUM_OUT_MONTH_HIGHS          GRF + 1462            // 25 ||  1
#define HUM_OUT_MONTH_LOWS           GRF + 1487            // 25 ||  1
#define HUM_OUT_YEAR_HIGHS           GRF + 1512            //  1 ||  1
#define HUM_OUT_YEAR_LOWS            GRF + 1513            //  1 ||  1

#define BAR_15_MIN                   GRF + 1514            // 24 ||  2
#define BAR_HOUR                     GRF + 1562            // 24 ||  2
#define BAR_DAY_HIGHS                GRF + 1610            // 24 ||  2
#define BAR_DAY_HIGH_TIMES           GRF + 1658            // 24 ||  2
#define BAR_DAY_LOWS                 GRF + 1706            // 24 ||  2
#define BAR_DAY_LOW_TIMES            GRF + 1754            // 24 ||  2
#define BAR_MONTH_HIGHS              GRF + 1802            // 25 ||  2
#define BAR_MONTH_LOWS               GRF + 1852            // 25 ||  2
#define BAR_YEAR_HIGHS               GRF + 1902            //  1 ||  2
#define BAR_YEAR_LOWS                GRF + 1904            //  1 ||  2

#define WIND_SPEED_10_MIN_AVG        GRF + 1906            // 24 ||  1
#define WIND_SPEED_HOUR_AVG          GRF + 1930            // 24 ||  1
#define WIND_SPEED_HOUR_HIGHS        GRF + 1954            // 24 ||  1
#define WIND_SPEED_DAY_HIGHS         GRF + 1978            // 24 ||  1
#define WIND_SPEED_DAY_HIGH_TIMES    GRF + 2002            // 24 ||  2
#define WIND_SPEED_DAY_HIGH_DIR      GRF + 2050            // 24 ||  1
#define WIND_SPEED_MONTH_HIGHS       GRF + 2074            // 25 ||  1
#define WIND_SPEED_MONTH_HIGH_DIR    GRF + 2099            // 25 ||  1
#define WIND_SPEED_YEAR_HIGHS        GRF + 2124            // 25 ||  1
#define WIND_SPEED_YEAR_HIGH_DIR     GRF + 2149            // 25 ||  1

#define WIND_DIR_HOUR                GRF + 2174            // 24 ||  1
#define WIND_DIR_DAY                 GRF + 2198            // 24 ||  1
#define WIND_DIR_MONTH               GRF + 2222            // 24 ||  1
#define WIND_DIR_DAY_BINS            GRF + 2246            //  8 ||  2
#define WIND_DIR_MONTH_BINS          GRF + 2262            //  8 ||  2

#define RAIN_RATE_1_MIN              GRF + 2278            // 24 ||  2
#define RAIN_RATE_HOUR               GRF + 2326            // 24 ||  2
#define RAIN_RATE_DAY_HIGHS          GRF + 2374            // 24 ||  2
#define RAIN_RATE_DAY_HIGH_TIMES     GRF + 2422            // 24 ||  2
#define RAIN_RATE_MONTH_HIGHS        GRF + 2470            // 25 ||  2
#define RAIN_RATE_YEAR_HIGHS         GRF + 2520            // 25 ||  2

#define RAIN_15_MIN                 GRF + 2570              // 24 || 1
#define RAIN_HOUR                   GRF + 2594              // 24 || 2
#define RAIN_STORM                  GRF + 2642              // 25 || 2
#define RAIN_STORM_START            GRF + 2692              // 25 || 2
#define RAIN_STORM_END              GRF + 2742              // 25 || 2
#define RAIN_DAY_TOTAL              GRF + 2792              // 25 || 2
#define RAIN_MONTH_TOTAL            GRF + 2842              // 25 || 2
#define RAIN_YEAR_TOTAL             GRF + 2892              // 25 || 2

#define ET_HOUR                      GRF + 2942             // 24 ||  1
#define ET_DAY_TOTAL                 GRF + 2966             // 25 ||  1
#define ET_MONTH_TOTAL               GRF + 2991             // 25 ||  2
#define ET_YEAR_TOTAL                GRF + 3041             // 25 ||  2

#define SOLAR_HOUR_AVG              GRF + 3091              // 24 || 2
#define SOLAR_DAY_HIGHS             GRF + 3139              // 24 || 2
#define SOLAR_DAY_HIGH_TIMES        GRF + 3187              // 24 || 2
#define SOLAR_MONTH_HIGHS           GRF + 3235              // 1  || 2
#define SOLAR_YEAR_HIGHS            GRF + 3237              // 1  || 2

#define UV_HOUR_AVG                 GRF + 3239              // 24 || 1
#define UV_MEDS_HOUR                GRF + 3263              // 24 || 1
#define UV_MEDS_DAY                 GRF + 3287              // 24 || 1
#define UV_DAY_HIGHS                GRF + 3311              // 24 || 1
#define UV_DAY_HIGH_TIMES           GRF + 3335              // 24 || 2
#define UV_MONTH_HIGHS              GRF + 3383              // 1  || 1
#define UV_YEAR_HIGHS               GRF + 3384              // 1  || 1

#define LEAF_HOUR                   GRF + 3385              // 24 || 1
#define LEAF_DAY_LOWS               GRF + 3409              // 24 || 1
#define LEAF_DAY_LOW_TIMES          GRF + 3433              // 24 || 2
#define LEAF_DAY_HIGHS              GRF + 3481              // 24 || 1
#define LEAF_DAY_HIGH_TIMES         GRF + 3505              // 24 || 2
#define LEAF_MONTH_LOWS             GRF + 3553              // 1  || 1
#define LEAF_MONTH_HIGHS            GRF + 3554              // 1  || 1
#define LEAF_YEAR_LOWS              GRF + 3555              // 1  || 1
#define LEAF_YEAR_HIGHS             GRF + 3556              // 1  || 1

#define SOIL_HOUR                   GRF + 3557              // 24 || 1
#define SOIL_DAY_LOWS               GRF + 3581              // 24 || 1
#define SOIL_DAY_LOW_TIMES          GRF + 3605              // 24 || 2
#define SOIL_DAY_HIGHS              GRF + 3653              // 24 || 1
#define SOIL_DAY_HIGH_TIMES         GRF + 3677              // 24 || 2
#define SOIL_MONTH_LOWS             GRF + 3725              // 1  || 1
#define SOIL_MONTH_HIGHS            GRF + 3726              // 1  || 1
#define SOIL_YEAR_LOWS              GRF + 3727              // 1  || 1
#define SOIL_YEAR_HIGHS             GRF + 3728              // 1  || 1
#define SOIL_YEAR_HIGHS_COMP        GRF + 3729              // 1  || 1

#define RX_PERCENTAGE               GRF + 3730              // 24 || 1

#define SAVE_MIN                    RX_PERCENTAGE+25        // 4080
#define SAVE_HOUR                   SAVE_MIN+1
#define SAVE_DAY                    SAVE_HOUR+1
#define SAVE_MONTH                  SAVE_HOUR+2
#define SAVE_YEAR                   SAVE_HOUR+3
#define SAVE_YEAR_COMP              SAVE_HOUR+4
#define BAUD_RATE                   SAVE_HOUR+5
#define DEFAULT_RATE_GRAPH          SAVE_HOUR+6
#define LCD_MODEL                   SAVE_HOUR+8
#define LCD_MODEL_COMP              SAVE_HOUR+9
#define LOG_AVERAGE_TEMPS           SAVE_HOUR+11            // MUST BE AT 4092


/* Definition of Davis LOOP data packet */
typedef struct t_RTDATA
{
    uint8_t yACK;           /* -1 ACK from stream                           */
    char    cL;             /* 0  character "L"                             */
    char    cO;             /* 1  character "O"                             */
    char    cO1;            /* 2  character "O"                             */
    char    cP;             /* 3  character "P" (RevA) or the current       */
                            /*    3-hour Barometer trend as follows:        */
                            /*    196 = Falling Rapidly                     */
                            /*    236 = Falling Slowly                      */
                            /*    0   = Steady                              */
                            /*    20  = Rising Slowly                       */
                            /*    60  = Rising Rapidly                      */
                            /* any other value is 3-hour data not available */
    uint8_t yPacketType;    /* 4 Always zero for current firmware release   */
    uint16_t    wNextRec;       /* 5 loc in archive memory for next data packet */
    uint16_t    wBarometer;     /* 7 Current barometer as (Hg / 1000)           */
    int16_t    wInsideTemp;    /* 9 Inside Temperature as (DegF / 10)          */
    uint8_t yInsideHum;     /* 11 Inside Humidity as percentage             */
    int16_t    wOutsideTemp;   /* 12 Outside Temperature as (DegF / 10)        */
    uint8_t yWindSpeed;     /* 14 Wind Speed                                */
    uint8_t yAvgWindSpeed;  /* 15 10-Minute Average Wind Speed              */
    uint16_t    wWindDir;       /* 16 Wind Direction in degress                 */
    int8_t yXtraTemps[7];  /* 18 Extra Temperatures                        */
    int8_t ySoilTemps[4];  /* 25 Soil Temperatures                         */
    int8_t yLeafTemps[4];  /* 29 Leaf Temperatures                         */
    uint8_t yOutsideHum;    /* 33 Outside Humidity                          */
    uint8_t yXtraHums[7];   /* 34 Extra Humidities                          */
    uint16_t    wRainRate;      /* 41 Rain Rate                                 */
    uint8_t yUVLevel;       /* 43 UV Level                                  */
    uint16_t    wSolarRad;      /* 44 Solar Radiation                           */
    uint16_t    wStormRain;     /* 46 Total Storm Rain                          */
    uint16_t    wStormStart;    /* 48 Start date of current storm               */
    uint16_t    wRainDay;       /* 50 Rain Today                                */
    uint16_t    wRainMonth;     /* 52 Rain this Month                           */
    uint16_t    wRainYear;      /* 54 Rain this Year                            */
    uint16_t    wETDay;         /* 56 Day ET                                    */
    uint16_t    wETMonth;       /* 58 Month ET                                  */
    uint16_t    wETYear;        /* 60 Year ET                                   */
    uint32_t    wSoilMoist;     /* 62 Soil Moistures                            */
    uint32_t    wLeafWet;       /* 66 Leaf Wetness                              */
    uint8_t yAlarmInside;   /* 70 Inside Alarm bits                         */
    uint8_t yAlarmRain;     /* 71 Rain Alarm bits                           */
    uint16_t    yAlarmOut;      /* 72 Outside Temperature Alarm bits            */
    uint8_t yAlarmXtra[8];  /* 74 Extra Temp/Hum Alarms                     */
    uint32_t    yAlarmSL;       /* 82 Soil and Leaf Alarms                      */
    uint8_t yXmitBatt;      /* 86 Transmitter battery status                */
    uint16_t    wBattLevel;     /* 87 Console Battery Level:                    */
                            /*    Voltage = ((wBattLevel * 300)/512)/100.0  */
    uint8_t yForeIcon;      /* 89 Forecast Icon                             */
    uint8_t yRule;          /* 90 Forecast rule number                      */
    uint16_t    wSunrise;       /* 91 Sunrise time (BCD encoded, 24hr)          */
    uint16_t    wSunset;        /* 93 Sunset time  (BCD encoded, 24hr)          */
    uint8_t yLF;            /* 95 Line Feed (\n) 0x0a                       */
    uint8_t yCR;            /* 96 Carraige Return (\r) 0x0d                 */
    uint16_t    WCRC;           /* 97 CRC check bytes (CCITT-16 standard)       */
} PACKED RTDATA;


/* Definition of Davis HILOW packet */
typedef struct t_HLDATA
{
    uint8_t yACK;           /* -1 ACK from stream                           */

    /* barometer */
    uint16_t    wBaroLoDay;     /* 0  Low barometer for today                   */
    uint16_t    wBaroHiDay;     /* 2  High barometer for today                  */
    uint16_t    wBaroLoMonth;   /* 4  Low barometer this month                  */
    uint16_t    wBaroHiMonth;   /* 6  High barometer this month                 */
    uint16_t    wBaroLoYear;    /* 8  Low barometer this year                   */
    uint16_t    wBaroHiYear;    /* 10 High barometer this year                  */
    uint16_t    wBaroLoTime;    /* 12 Low barometer time of day                 */
    uint16_t    wBaroHiTime;    /* 14 High barometer time of day                */

    /* wind speed */
    uint8_t yWindHiDay;     /* 16 Highest wind speed for today              */
    uint16_t    wWindHiTime;    /* 18 Highest wind speed time of day            */
    uint8_t yWindHiMonth;   /* 18 Highest wind speed for the month          */
    uint8_t yWindHiYear;    /* 20 Highest wind speed for the year           */

    /* inside temperatures */
    int16_t    wInTempHiDay;   /* 21 Inside high temp for today                */
    int16_t    wInTempLoDay;   /* 23 Inside low temp for today                 */
    int16_t    wInTempHiTime;  /* 25 Time of Inside high temp for today        */
    int16_t    wInTempLoTime;  /* 27 Time of Inside low temp for today         */
    int16_t    wInTempLoMonth; /* 29 Inside low temp for the month             */
    int16_t    wInTempHiMonth; /* 31 Inside high temp for the month            */
    int16_t    wInTempLoYear;  /* 33 Inside low temp for the year              */
    int16_t    wInTempHiYear;  /* 35 Inside high temp for the year             */

    /* Inside Humidity */
    uint8_t yInHumHiDay;    /* 37 Inside high humidity for the day          */
    uint8_t yInHumLoDay;    /* 38 Inside low humidity for the day           */
    uint16_t    wInHumHiTime;   /* 39 Inside high humidity time for today       */
    uint16_t    wInHumLoTime;   /* 41 Inside low humidity time for today        */
    uint8_t yInHumHiMonth;  /* 43 Inside high humidity for the month        */
    uint8_t yInHumLoMonth;  /* 44 Inside low humidity for the month         */
    uint8_t yInHumHiYear;   /* 45 Inside high humidity for the year         */
    uint8_t yInHumLoYear;   /* 46 Inside low humidity for the year          */

    /* outside temperatures */
    int16_t    wTempLoDay;     /* 49 Outside low temp for today                */
    int16_t    wTempHiDay;     /* 47 Outside high temp for today               */
    int16_t    wTempLoTime;    /* 53 Time of Outside low temp for today        */
    int16_t    wTempHiTime;    /* 51 Time of Outside high temp for today       */
    int16_t    wTempHiMonth;   /* 57 Outside high temp for the month           */
    int16_t    wTempLoMonth;   /* 55 Outside low temp for the month            */
    int16_t    wTempHiYear;    /* 61 Outside high temp for the year            */
    int16_t    wTempLoYear;    /* 59 Outside low temp for the year             */

    /* dew point */
    int16_t    wDewLoDay;      /* 63 dew point low for today                   */
    int16_t    wDewHiDay;      /* 65 dew point high for today                  */
    int16_t    wDewLoTime;     /* 67 Time of dew point low for today           */
    int16_t    wDewHiTime;     /* 69 Time of dew point high for today          */
    int16_t    wDewHiMonth;    /* 71 Highest dew point this month              */
    int16_t    wDewLoMonth;    /* 73 Lowest dew point this month               */
    int16_t    wDewHiYear;     /* 75 Highest dew point for the year            */
    int16_t    wDewLoYear;     /* 77 Lowest dew point for the year             */

    /* wind chill */
    int16_t    wChillLoDay;    /* 79 wind chill low for today                  */
    int16_t    wChillLoTime;   /* 81 Time of wind chill low for today          */
    int16_t    wChillLoMonth;  /* 83 Lowest wind chill this month              */
    int16_t    wChillLoYear;   /* 85 Lowest wind chill for the year            */

    /* heat indices */
    int16_t    wHeatHiDay;     /* 87 Heat index high for today                 */
    int16_t    wHeatHiTime;    /* 89 Time of heat index high for today         */
    int16_t    wHeatHiMonth;   /* 91 Heat index high for the month             */
    int16_t    wHeatHiYear;    /* 93 Heat index high for the year              */

    /* THSW indices */
    uint16_t    wTHSWHiDay;     /* 95 THSW index high for today                 */
    uint16_t    wTHSWHiTime;    /* 97 Time of THSW index high for today         */
    uint16_t    wTHSWHiMonth;   /* 99 THSW index high for the month             */
    uint16_t    wTHSWHiYear;    /* 101 THSW index high for the year             */

    /* Solar Radiation */
    uint16_t    wSolarHiDay;    /* 103 Solar rad high for today                 */
    uint16_t    wSolarHiTime;   /* 105 Time of Solar rad high for today         */
    uint16_t    wSolarHiMonth;  /* 107 Solar rad high for the month             */
    uint16_t    wSolarHiYear;   /* 109 Solar rad high for the year              */

    /* UV Index */
    uint8_t yUVHiDay;       /* 111 UV high for today                        */
    uint16_t    wUVHiTime;      /* 112 Time of UV high for today                */
    uint8_t yUVHiMonth;     /* 114 UV high for the month                    */
    uint8_t yUVHiYear;      /* 115 UV high for the year                     */

    /* Rain Rate */
    uint16_t    wRainHiDay;     /* 116 Rain Rate high for today                 */
    uint16_t    wRainHiTime;    /* 118 Time of Rain Rate high for today         */
    uint16_t    wRainHiHour;    /* 120 Highest Rain Rate this hour              */
    uint16_t    wRainHiMonth;   /* 122 Highest Rain Rate this month             */
    uint16_t    wRainHiYear;    /* 124 Highest Rain Rate this year              */

    uint8_t yExtraLeaf[150];/* 126 Extra/Leaf/Soil Temperatures             */
    uint8_t yExtraTemps[80];/* 276 Extra outside temp/Humidities            */
    uint8_t ySoilMoist[40]; /* 356 Soil Moisture section                    */
    uint8_t yLeafWet[40];   /* 396 Leaf Wetness section                     */
    uint16_t    wUnknown;       /* 436 not documented                           */
    uint16_t    WCRC;           /* 438 CRC check bytes (CCITT-16 standard)      */
} PACKED HLDATA;

/* Definition of Davis EEPROM data packet */
typedef struct t_EEDATA
{
    uint8_t szDumm[177];    /* stuff not used in this version       */

} PACKED EEDATA;


#ifdef __cplusplus
}
#endif /* __cplusplus    */
#endif /* __DHANDLER_H__ */
