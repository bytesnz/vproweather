vproweather
================
Initial version written by Joe Jaworski (http://www.joejaworski.com/weather/)

Additions made by Jack Farley (https://github.com/bytesnz/wproweather)

```
vproweather v1.3.4
https://github.com/bytesnz/vproweather
Original work by Joe Jaworski http://www.joejaworski.com/weather/

Usage: vproweather [Options] Device
Options:
 -x, --get-realtime     Get real time weather data.
 -l, --get-highlow      Get Highs/Lows data.
 -g, --get-graph        Get graph data.
 -a, --get-archive=arc  Print either:
                          - all the archive records (default)
                          - all the records since arc date
                            (in yyyy-mm-dd[THH:MM format]
                          - the last arc records
 -A, --archive-header   Print the archive data header with the
                        archive records
 -i, --get-interval     Get the current archive interval
 -t, --get-time         Get weather station time.
 -s, --set-time         Set weather station time to system time.
 -o, --bklite-on        Turn backlite on.
 -f, --bklite-off       Turn backlite off.
 -r, --version          Query for Davis firmware version string.
 -m, --model            Query for weather station model name.
 -d, --delay=num        Cmd Delay in 1/10ths seconds. Default is 10 (1 sec).
 -v, --verbose          Verbose mode.
 Device                 Serial Device. Required parameter.

Examples:
vproweather --get-realtime /dev/ttyp0 > rtwdata.txt
Gets real time data set to file rtwdata.txt from serial device ttyp0
vproweather --verbose --bklite-on /dev/ttyp0
Turns the LCD backlite On, illuminating the display.
```

## Real Time Example Output
```
rtNextArchiveRecord = 0x0139
rtBaroTrend = Steady
rtBaroTrendImg = baro_s
rtBaroCurr = 30.08
rtInsideTemp = 80.6
rtInsideHum = 67
rtOutsideTemp = 76.9
rtWindSpeed = 2
rtWindAvgSpeed = 2.2
rtWind2mAvgSpeed = 1.8
rtWindDir = 284
rtWindDirRose = NNE
rtWind10mGustMaxSpeed = 5
rtWind10mGustMaxDir = 267
rtWind10mGustMaxDirRose = NNW
rtOutsideHum = 73
rtRainRate = 0.00
rtIsRaining = no
rtUVLevel = 0.0
rtSolarRad = 65
rtHeatIndex = 19712
rtWindChill = 20224
rtThswIndex = 0
rtRainStorm = 0.00
rtStormStartDate = n/a
rt15mRain = 0.00
rtHourRain = 389.12
rtDayRain = 0.00
rtMonthRain = 0.00
rtYearRain = 6.77
rtDayET = 152
rtMonthET = 87
rtXmitBattt = 0
rtBattVoltage = 4.4
rtForeIcon = 6
rtForeRule = 44
rtForecast = Increasing clouds with little temperature change. Precipitation possible within 24 to 48 hrs.
rtSunrise = 06:12
rtSunset = 17:57
```

## His/Lows Example Output
```
hlBaroLoDay = 30.08
hlBaroHiDay = 30.15
hlBaroLoMonth = 29.98
hlBaroHiMonth = 30.23
hlBaroLoYear = 29.54
hlBaroHiYear = 30.23
hlBaroLoTime = 16:15
hlBaroHiTime = 10:04
hlWindHiDay = 9
hlWindHiTime = 12:37
hlWindHiMonth = 33
hlWindHiYear = 39
hlInTempHiDay = 80.6
hlInTempLoDay = 69.0
hlInTempHiTime = 17:10
hlInTempLoTime = 05:30
hlInTempLoMonth = 69.0
hlInTempHiMonth = 83.0
hlInTempLoYear = 64.3
hlInTempHiYear = 96.0
hlInHumHiDay = 75
hlInHumLoDay = 60
hlInHumHiTime = 11:51
hlInHumLoTime = 01:40
hlInHumHiMonth = 80
hlInHumLoMonth = 40
hlInHumHiYear = 87
hlInHumLoYear = 26
hlOutTempHiDay = 79.0
hlOutTempLoDay = 56.6
hlOutTempHiTime = 13:54
hlOutTempLoTime = 04:43
hlOutTempLoMonth = 56.6
hlOutTempHiMonth = 84.5
hlOutTempHiYear = 96.0
hlOutTempLoYear = 56.4
hlDewLoDay = 48
hlDewHiDay = 70
hlDewLoTime = 06:24
hlDewHiTime = 11:03
hlDewHiMonth = 71
hlDewLoMonth = 48
hlDewHiYear = 83
hlDewLoYear = 46
hlChillLoDay = 57
hlChillLoTime = 04:33
hlChillLoMonth = 57
hlChillLoYear = 56
hlHeatHiDay = 82
hlHeatHiTime = 13:54
hlHeatHiMonth = 84
hlHeatHiYear = 118
hlSolarHiDay = 84.9
hlSolarHiTime = 11:57
hlSolarHiMonth = 85.6
hlSolarHiYear = 140.3
hlUVHiDay = 5.1
hlUVHiTime = 11:35
hlUVHiMonth = 5.4
hlUVHiYear = 13.6
hlRainRateHiDay = 0.00
hlRainRateHiTime = n/a
hlRainRateHiHour = 0.00
hlRainRateHiMonth = 0.00
hlRainRateHiYear = 5.38
```

## Graph Data Example Output
```
grTimeRef10Min = 14:50,15:00,15:10,15:20,15:30,15:40,15:50,16:00,16:10,16:20,16:30,16:40,16:50,17:00,17:10,17:20,17:30,17:40,17:50,18:00,18:10,18:20,18:30,18:40
grTimeRef15Min = 12:45,13:00,13:15,13:30,13:45,14:00,14:15,14:30,14:45,15:00,15:15,15:30,15:45,16:00,16:15,16:30,16:45,17:00,17:15,17:30,17:45,18:00,18:15,18:30
grTimeRefHours = 19:00,20:00,21:00,22:00,23:00,00:00,01:00,02:00,03:00,04:00,05:00,06:00,07:00,08:00,09:00,10:00,11:00,12:00,13:00,14:00,15:00,16:00,17:00,18:00
grTimeRefDays = 2017-08-13,2017-08-14,2017-08-15,2017-08-16,2017-08-17,2017-08-18,2017-08-19,2017-08-20,2017-08-21,2017-08-22,2017-08-23,2017-08-24,2017-08-25,2017-08-26,2017-08-27,2017-08-28,2017-08-29,2017-08-30,2017-08-31,2017-09-01,2017-09-02,2017-09-03,2017-09-04,2017-09-05
grTimeRefMonths = 9,10,11,12,1,2,3,4,5,6,7,8,9,10,11,12,1,2,3,4,5,6,7,8
grTimeRefYears = 1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015,2016
grInTempByHours = 78,77,76,75,74,73,72,71,70,70,69,69,69,69,72,74,76,77,78,78,79,79,80,79
grInTempHiByDays = 89,95,94,80,86,88,84,81,80,79,79,80,80,78,77,79,81,82,80,80,77,79,83,83
grInTempHiByDaysTime = 00:52,01:31,01:31,01:39,23:53,00:24,00:00,00:00,01:46,04:41,07:35,17:00,17:11,17:01,17:04,17:01,17:16,17:17,17:14,17:08,14:21,17:35,16:51,13:38
grInTempLoByDays = 73,72,70,68,73,69,71,66,64,70,70,72,70,71,71,70,70,71,70,70,72,72,69,70
grInTempLoByDaysTime = 22:30,21:43,22:48,21:32,13:54,21:37,21:26,20:16,21:03,21:43,21:46,00:00,07:01,07:04,07:50,07:32,07:29,06:59,07:16,07:24,05:22,07:28,07:11,06:57
grInTempHiByMonths = 80,84,86,90,89,90,92,93,94,89,84,83,83,84,87,87,90,93,94,96,95,92,93,95
grInTempLoByMonths = 61,64,67,73,69,77,78,80,79,74,69,65,65,65,67,67,76,76,80,79,77,73,66,64
grOutTempByHours = 74,70,67,67,66,63,60,58,61,61,57,59,62,73,76,76,78,78,78,79,78,78,77,75
grOutTempHiByDays = 85,83,81,84,87,89,82,77,77,81,78,78,80,85,80,81,85,81,82,78,78,77,85,82
grOutTempHiByDaysTime = 00:07,02:33,06:07,23:58,23:59,00:18,00:15,04:02,01:45,01:06,04:37,03:28,10:09,12:54,11:08,11:23,12:24,13:45,10:13,14:04,12:44,15:21,14:02,13:50
grOutTempLoByDays = 68,67,64,65,72,66,67,60,60,63,62,65,62,61,62,63,63,62,56,60,64,65,59,62
grOutTempLoByDaysTime = 22:04,20:26,22:40,19:08,14:16,21:24,20:39,21:15,21:06,16:34,19:34,23:59,02:01,05:49,05:13,06:32,03:10,06:10,06:13,05:11,02:24,23:59,05:47,05:42
grOutTempHiByMonths = 85,90,95,92,103,95,102,95,93,95,87,85,86,89,89,90,90,95,91,96,96,95,93,92
grOutTempLoByMonths = 50,55,57,64,60,68,69,70,73,63,57,54,53,55,58,61,66,67,71,72,67,66,61,56
grDewByHours = 67,64,62,62,62,59,56,53,58,57,53,50,49,55,62,65,69,70,66,68,68,68,67,67
grDewHiByDays = 73,74,72,70,69,73,73,65,62,67,69,67,65,67,63,67,69,68,67,70,71,66,66,69
grDewHiByDaysTime = 01:56,04:57,06:34,02:59,03:52,00:55,01:42,00:00,08:10,05:08,02:37,01:56,17:37,13:25,12:39,17:43,18:06,14:44,16:22,15:50,12:08,07:39,17:35,11:43
grDewLoByDays = 58,61,63,47,51,59,64,56,52,57,59,61,56,53,51,52,53,55,53,54,62,57,48,49
grDewLoByDaysTime = 22:42,00:00,19:13,22:31,00:00,00:02,20:46,20:45,19:29,00:00,19:19,23:13,09:42,12:52,20:45,21:26,00:00,05:19,04:20,05:02,02:10,22:15,14:04,09:06
grDewHiByMonths = 71,73,74,78,80,81,83,82,83,81,74,72,73,73,76,77,79,82,82,83,82,81,76,75
grDewLoByMonths = 45,42,46,53,45,60,59,63,66,52,42,43,45,43,46,52,59,58,66,62,63,55,46,47
grChillLoByHours = 74,70,67,67,66,63,60,58,58,60,57,57,59,62,73,75,75,78,76,78,78,78,77,75
grChillLoByDays = 68,67,64,65,72,66,67,60,58,63,62,65,62,61,61,61,63,62,56,60,64,65,59,62
grChillLoByDaysTime = 21:58,20:00,22:31,19:08,13:37,21:21,20:39,21:02,18:44,16:27,17:43,23:56,01:48,05:46,05:38,05:09,02:46,06:04,06:13,04:59,02:13,23:39,05:12,04:38
grChillLoByMonths = 50,54,55,64,60,68,69,70,72,63,56,51,52,55,58,61,66,67,70,70,67,62,61,56
grHeatHiByHours = 76,71,69,68,67,64,60,58,61,61,57,58,62,73,77,78,80,81,80,81,80,80,79,78
grHeatHiByDays = 89,89,85,82,87,91,85,78,77,82,80,80,80,86,80,82,86,83,82,80,81,79,84,84
grHeatHiByDaysTime = 00:11,05:03,06:06,03:01,23:32,00:46,00:18,00:16,01:22,01:06,02:38,03:28,10:01,12:55,11:03,11:23,12:25,13:45,10:15,13:52,12:14,14:25,14:19,11:46
grHeatHiByMonths = 86,92,96,99,107,109,116,111,120,110,89,89,86,87,93,92,101,113,114,118,114,108,98,100
grInHumByHours = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grInHumHiByDays = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grInHumHiByDaysTime = 10:47,23:41,23:49,14:52,02:35,22:43,19:07,21:53,22:08,23:53,00:00,01:50,00:00,08:06,07:44,18:00,19:01,20:22,00:00,21:33,12:39,00:00,07:37,11:48
grInHumLoByDays = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grInHumLoByDaysTime = 03:17,01:26,01:31,23:43,23:58,00:21,23:52,23:51,01:31,00:40,07:56,03:56,10:27,12:54,21:53,22:28,12:40,00:00,10:14,09:32,03:00,20:46,14:19,13:44
grInHumHiByMonths = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grInHumLoByMonths = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grOutHumByHours = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grOutHumHiByDays = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grOutHumHiByDaysTime = 16:39,17:37,21:02,15:27,04:00,20:53,19:35,21:24,21:25,21:25,19:38,23:57,02:06,06:04,06:39,06:38,18:34,21:19,06:36,23:44,03:25,07:13,06:02,22:50
grOutHumLoByDays = 10:04,12:52,11:05,82:24,161:66,115:77,161:73,113:24,118:10,97:75,102:90,192:60,72:33,130:89,161:81,23:27,00:08,00:02,00:11,00:59,01:46,01:11,23:51,03:23
grOutHumLoByDaysTime = 00:00,00:04,02:01,23:27,00:08,00:02,00:11,00:59,01:46,01:11,23:51,03:23,10:04,12:52,11:05,11:23,12:44,08:39,10:10,09:32,18:29,20:02,14:04,09:07
grOutHumHiByMonths = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grOutHumLoByMonths = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grBarBy15Min = 30.09,30.09,30.11,30.11,30.10,,30.10,30.09,30.09,30.09,30.08,30.09,30.08,30.08,30.08,30.08,30.08,30.08,30.08,30.08,30.08,30.08,30.09,30.09
grBarByHours = 30.13,30.13,30.14,30.14,30.13,30.12,30.12,30.11,,30.10,30.11,30.12,30.13,30.14,30.14,30.15,30.14,30.12,30.11,30.09,30.09,30.08,30.08,30.09
grBarHiByDays = 30.10,30.11,,30.04,30.03,30.11,30.18,30.18,30.17,30.00,30.00,30.06,30.11,30.15,30.15,30.13,30.07,30.08,30.10,30.09,30.05,30.18,30.23,30.21
grBarHiByDaysTime = 01:18,00:13,00:26,00:39,23:54,23:27,23:59,00:02,00:06,00:00,13:39,22:53,23:57,08:18,09:27,00:13,08:13,21:04,08:36,08:53,09:07,23:25,08:59,00:02
grBarLoByDays = 30.03,29.99,,29.95,29.92,29.99,30.06,30.10,30.00,29.87,29.92,29.94,30.00,30.02,30.04,29.98,29.93,29.97,30.01,29.98,29.98,30.01,30.12,30.08
grBarLoByDaysTime = 08:16,06:34,06:02,05:45,05:08,05:36,05:40,04:37,23:31,20:10,05:25,14:49,14:39,15:40,15:27,17:56,15:47,03:08,14:13,16:25,14:18,03:05,14:05,15:15
grBarHiByMonths = 30.29,30.33,30.29,30.14,30.23,30.00,29.97,30.02,30.03,30.10,30.32,30.39,30.30,30.31,30.32,30.12,30.11,30.04,30.01,29.99,30.02,30.12,30.22,30.18
grBarLoByMonths = 29.94,29.97,29.86,29.86,29.73,29.73,29.71,29.67,29.75,29.81,29.88,30.02,29.96,29.95,29.90,29.81,29.78,29.75,29.74,29.70,29.54,29.86,29.90,29.87
grWindAvgBy10Min = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grWindAvgByHours = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grWindHiByDays = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grWindHiByDaysTime = 03:10,05:12,07:02,22:33,17:41,04:31,10:20,06:00,03:26,22:27,01:30,03:40,16:29,15:51,17:51,15:59,14:18,00:20,12:42,10:08,13:39,17:32,16:34,14:52
grWindHiByMonths = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grWindHiByYears = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grWindDirByHours = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grWindDirByDays = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grWindDirByMonths = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
grRainRateByMinutes = 327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68
grRainRateByHours = 0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00
grRainRateHiByDays = 327.68,327.68,,327.68,327.68,327.68,327.70,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68
grRainRateHiByDaysTime = ,,,,,,18:00,,,,,,,,,,,,,,,,,
grRainRateHiByMonths = 340.34,340.48,327.68,328.78,340.62,339.56,327.70,333.16,342.84,327.70,327.92,328.20,328.86,327.68,327.68,341.72,334.16,335.36,334.88,338.44,331.70,335.12,334.42,327.70
grRainRateHiByYears = 655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,492.26,342.84
grRainByHour = 0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00
grRainStorms = 327.84,327.82,327.72,327.82,327.80,327.76,331.80,327.80,328.20,329.76,327.92,327.98,329.44,329.06,332.04,328.38,328.22,328.10,327.90,327.72,328.00,327.74,328.16,327.72
grRainStormsStartDate = 2016-06-01,2016-06-02,2016-06-06,2016-06-14,2016-07-22,2016-07-30,2016-10-01,2016-10-06,2016-11-18,2016-12-20,2017-01-03,2017-01-04,2017-01-06,2017-01-10,2017-02-11,2017-02-22,2017-03-10,2017-03-18,2017-03-26,2017-04-02,2017-04-22,2017-05-14,2017-05-17,2017-06-16
grRainStormsEndDate = 2016-06-01,2016-06-02,2016-06-06,2016-06-14,2016-07-23,2016-07-30,2016-10-01,2016-10-06,2016-11-18,2016-12-21,2017-01-03,2017-01-04,2017-01-06,2017-01-10,2017-02-12,2017-02-23,2017-03-10,2017-03-18,2017-03-26,2017-04-02,2017-04-22,2017-05-14,2017-05-18,2017-06-16
grRainByDay = 327.68,327.68,327.68,327.68,327.68,327.68,327.72,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68,327.68
grRainByMonth = 328.12,328.56,327.68,328.22,331.94,330.86,327.70,328.26,348.76,327.70,327.80,328.22,327.88,327.68,327.68,331.92,328.22,329.76,331.38,332.76,328.88,328.04,328.22,327.72
grRainByYear = 655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,655.35,370.78,354.48
```

## Archive Data Example Output (with Archive Header)
```
date,time,outside temp (°F),outside temp high (°F), outside temp low (°F),insideTemp (°F),extra temp1 (°F), extra temp2 (°F), extra temp3 (°F),inside humidity (%),outside humidity (%),extra humidity 1 (%),extra humidity 2 (%),rainfall (clicks),highest rain rate (clicks/hr),barometer (inches Hg),forecast at end of period,number of wind samples,average wind speed (mph),prevailing wind direction (°),prevailing wind rose,highest wind speed (mph),highest wind direction (°),higest wind rose,average solar radiation (W/m^2),maximum solar radiation (W/m^2),average uv index,max uv,accumulated et (in),leaf temp 1 (°F),leaf temp 2 (°F),leaf wetness 1,leaf wetness 2,soil temp 1 (°F),soil temp 2 (°F),soil temp 3 (°F),soil temp 4 (°F),soil moisture 1 (cb),soil moisture 2 (cb), soil moisture 3 (cb), soil moisture 4 (cb)
2017-09-04,19:00,72.9,73.7,72.9,78.2,,,,75,65,,,,,30.155,"Increasing clouds with little temperature change. Precipitation possible within 24 to 48 hrs.",1181,11,10,NNW,24,11,NNW,0,,0,,0.003,,,,,,,,,,,,
2017-09-04,20:00,71.4,72.9,71.4,77.4,,,,70,61,,,,,30.174,"Mostly clear with little temperature change.",1220,7,9,NNW,18,10,NNW,0,,0,,0.002,,,,,,,,,,,,
2017-09-04,21:00,71.8,71.8,71.2,75.2,,,,61,61,,,,,30.191,"Mostly clear with little temperature change.",1157,3,9,NNW,12,9,NNW,0,,0,,0.002,,,,,,,,,,,,
2017-09-04,22:00,71.1,71.9,71.1,74.5,,,,61,59,,,,,30.206,"Mostly clear with little temperature change.",1181,3,7,NNW,11,10,NNW,0,,0,,0.002,,,,,,,,,,,,
2017-09-04,23:00,69.3,71.1,69.3,73.9,,,,62,58,,,,,30.216,"Mostly clear with little temperature change.",1184,5,6,NNW,19,8,NNW,0,,0,,0.003,,,,,,,,,,,,
2017-09-05,00:00,67.1,69.3,67.1,73.4,,,,66,58,,,,,30.210,"Partly cloudy with little temperature change.",1191,5,6,NNW,20,6,NNW,0,,0,,0.003,,,,,,,,,,,,
2017-09-05,01:00,65.4,67.1,65.4,72.7,,,,67,57,,,,,30.197,"Increasing clouds with little temperature change. Precipitation possible within 24 to 48 hrs.",1167,6,6,NNW,19,6,NNW,0,,0,,0.003,,,,,,,,,,,,
2017-09-05,02:00,64.2,65.4,64.2,72.1,,,,67,57,,,,,30.181,"Increasing clouds with little temperature change. Precipitation possible within 24 to 48 hrs.",1184,6,6,NNW,19,6,NNW,0,,0,,0.002,,,,,,,,,,,,
2017-09-05,03:00,64.0,64.2,63.7,71.5,,,,68,57,,,,,30.162,"Increasing clouds and warmer. Precipitation possible within 12 to 24 hrs. Windy.",1160,5,6,NNW,18,6,NNW,0,,0,,0.002,,,,,,,,,,,,
2017-09-05,04:00,63.2,64.0,63.2,71.0,,,,69,57,,,,,30.156,"Increasing clouds and warmer. Precipitation possible within 12 to 24 hrs. Windy.",1197,4,6,NNW,17,6,NNW,0,,0,,0.002,,,,,,,,,,,,
```
