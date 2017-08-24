vproweather
================
Initial version written by Joe Jaworski (http://www.joejaworski.com/weather/)

Additions made by Jack Farley (https://github.com/bytesnz/wproweather)

```
Usage: vproweather [Options] Device
Options:
 -x, --get-realtime    Get real time weather data.
 -l, --get-highlow     Get Highs/Lows data.
 -g, --get-graph       Get graph data.
 -t, --get-time        Get weather station time.
 -s, --set-time        Set weather station time to system time.
 -o, --bklite-on       Turn backlite on.
 -f, --bklite-off      Turn backlite off.
 -r, --version         Query for Davis firmware version string.
 -m, --model           Query for weather station model name.
 -d, --delay=num       Cmd Delay in 1/10ths seconds. Default is 10 (1 sec).
 -v, --verbose         Verbose mode.
 Device                Serial Device. Required parameter.

Examples:
vproweather --get-realtime /dev/ttyp0 > rtwdata.txt
Gets real time data set to file rtwdata.txt from serial device ttyp0
vproweather --verbose --bklite-on /dev/ttyp0
Turns the LCD backlite On, illuminating the display.
```
