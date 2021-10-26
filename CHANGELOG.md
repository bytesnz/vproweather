# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [unreleased]

### Fixed

* Fixed size of 5th unused variable in the LOOP2 real time data packet. This
  error would have affected the following fields since v1.2.0:
  * Outside Humidity
  * Heat Index
  * Wind Chill
  * THSW Index
  * Rain Rate
  * UV
  * Solar Radiation
  * Storm Rain
  * Start Date of current Storm
  * Daily Rain
  * Last 15-min Rain
  * Last Hour Rain
  * Daily ET
  * Last 24-Hour Rain
  * Barometric Reduction Method
  * User-entered Barometric Offset
  * Barometric calibration number
  * Barometric Sensor Raw Reading
  * Absolute Barometric Pressure
  * Altimeter Setting

## [1.3.6] - 2018-02-13

Fixed order of humidty in archive header

* @amiskelly fixed the order of the inside/outside humidity in the archive
  header


## [1.3.2] - 2017-09-08

Add option for printing archive header

* Add option (-a, --archive-header) to print the archive header instead of
  always printing


## [1.3.1] - 2017-09-08

Minor fix to ignore 0x0a0d at end of data

* `0x0a0d` is sent along with the last archive data packet, so have to ignore
  it when checking the length of the data packet
* Minor modification of the verbose sending command message


## [1.3.0] - 2017-09-08

Added fetching and printing of archive records

* *Breaking change* Changed output format of dates and time values to be
  inline with ISO 8601
* Added fetching and printing of archive records. "Dash values" (values for
  which a record is missing) are blank. You can either specify a date from
  which to print records from or the number of records to print
* Ensured "dash values" are blank in other outputs


## [1.2.0] - 2017-09-08

The inclusion of LOOP2 data

* increased the 10 minute average wind speed resolution to 0.1MPH
* added the 2 minute average wind speed
* added information on the biggest gust in the last 10 minutes
* added the heat index, wind chill and THSW indexes
* added the amount of rain in the last 15 minutes and last hour


## [1.1.0] - 2017-09-08

Original version from Joe Jaworski

[0.6.2]: https://github.com/bytesnz/vproweather/compare/v0.6.1...v0.6.2
[0.6.1]: https://github.com/bytesnz/vproweather/compare/v0.6.0...v0.6.1
[0.6.0]: https://github.com/bytesnz/vproweather/compare/v0.5.0...v0.6.0
[0.5.0]: https://github.com/bytesnz/vproweather/compare/v0.4.3...v0.5.0
[0.4.3]: https://github.com/bytesnz/vproweather/compare/v0.4.2...v0.4.3
[0.4.2]: https://github.com/bytesnz/vproweather/compare/v0.4.1...v0.4.2
[0.4.1]: https://github.com/bytesnz/vproweather/compare/v0.4.0...v0.4.1
[0.4.0]: https://github.com/bytesnz/vproweather/compare/v0.3.6...v0.4.0
[0.3.6]: https://github.com/bytesnz/vproweather/compare/v0.3.5...v0.3.6
[0.3.5]: https://github.com/bytesnz/vproweather/compare/v0.3.4...v0.3.5
[0.3.4]: https://github.com/bytesnz/vproweather/compare/v0.3.3...v0.3.4
[0.3.3]: https://github.com/bytesnz/vproweather/compare/v0.3.2...v0.3.3
[0.3.2]: https://github.com/bytesnz/vproweather/compare/v0.3.1...v0.3.2
[0.3.1]: https://github.com/bytesnz/vproweather/compare/v0.3.0...v0.3.1
[0.3.0]: https://github.com/bytesnz/vproweather/compare/v0.2.1...v0.3.0
[0.2.1]: https://github.com/bytesnz/vproweather/compare/v0.1.3...v0.2.1
[0.1.3]: https://github.com/bytesnz/vproweather/compare/v0.1.2...v0.1.3
[0.1.2]: https://github.com/bytesnz/vproweather/compare/v0.0.1...v0.1.2
[0.0.1]: https://github.com/bytesnz/vproweather/tree/v0.0.1
