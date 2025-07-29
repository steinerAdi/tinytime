/**
 * @file tinytime.h
 * @author Adrian STEINER (steia19@bfh.ch)
 * @brief A lightweight time.h library replacement for small embedded systems
 * @version 0.1
 * @date 10-03-2025
 *
 * @copyright (C) 2025 Adrian STEINER
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https: //www.gnu.org/licenses/>.
 *
 */

#ifndef TINY_TIME_H
#define TINY_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
  TINY_JAN = 1,
  TINY_FEB,
  TINY_MAR,
  TINY_APR,
  TINY_MAY,
  TINY_JUN,
  TINY_JUL,
  TINY_AUG,
  TINY_SEP,
  TINY_OCT,
  TINY_NOV,
  TINY_DEC,
  TINY_MAX_MONTHS
} TINY_MONTHS;

typedef enum {
  TINY_SUN = 0,
  TINY_MON,
  TINY_TUE,
  TINY_WED,
  TINY_THU,
  TINY_FRI,
  TINY_SAT,
  TINY_MAX_WEAKDAYS
} TINY_WEAK_DAYS;

/**
 * @brief Structure to handle the calendar date and time
 *
 */
typedef struct {
  /* Time informations */
  uint8_t sec;  ///< Seconds after minute ranged from 0 - 59
  uint8_t min;  ///< Minutes after hour ranged from 0 - 59
  uint8_t hour; ///< Hour since midnight ranged from 0 - 23
  /* Date informations */
  uint8_t monthDay; ///< Day in the month range depending from 1 to the current month
  uint8_t month;    ///< Month of the current year, ranged from 1 - 12
  uint16_t year;    ///< Current year since year 0
  /* Additional Date informations */
  uint8_t weakDay;  ///< Current weak day from sunday to saturday
  uint16_t yearDay; ///< Current year day
} tinyTimeType;

/**
 * @brief Tiny Unix Time Type
 *
 */
typedef uint64_t tinyUnixType;

#define TINY_SEC_MAX ((uint8_t)59)    ///< Max second value
#define TINY_MINUTE_MAX ((uint8_t)59) ///< Max minute value
#define TINY_HOUR_MAX ((uint8_t)23)   ///< Max hour value

#define TINY_UNIX_YEAR_BEGIN ((uint16_t)1970) ///< Starting year of Unix time epoch
#define TINY_ONE_MIN_IN_SEC ((uint8_t)60)     ///< One minute in seconds
#define TINY_ONE_HOUR_IN_SEC ((uint16_t)3600) ///< One hour in seconds
#define TINY_ONE_DAY_IN_SEC ((uint32_t)86400) ///< One day in seconds
#define TINY_ONE_YEAR_IN_DAYS ((uint16_t)365) ///< One basic year in days

/**
 * @brief Get the current unix time from a time
 *
 * @param tm Base time type to convert to unix time
 * @return tinyUnixType Unix time of the tm time and date or UINT64_MAX in case of an error
 */
tinyUnixType tiny_getUnixTime(const tinyTimeType *tm);

/**
 * @brief Convert the unix time to the human readable format tinyTimeType
 *
 * @param tm The reference to a tinyTimeType structure instance
 * @param unixTime The unix time to convert
 */
void tiny_getTimeType(tinyTimeType *tm, const tinyUnixType unixTime);

/**
 * @brief Returns a string converted human readable date format.
 *
 * The returned string has the following format:
 *
 * ``Www dd Mmm yyyy hh:mm:ss\0``
 *
 * Where Www is the weekday,
 * dd the day of the month, Mmm the month (in letters), yyyy the year
 * and hh:mm:ss the time.
 *
 * The static c-array buffer has a size of 25.
 *
 * @param tm Pointer to an object of type tinyTimeType that contains a time value
 * @return A 25 byte sized C-string containing the date and time information in a human-readable format.
 */
const char *tiny_getFormat(const tinyTimeType *tm);

/**
 * @brief Checks if the given year is a leap year.
 *
 * @param year the year to check if it is a leap year
 * @return uint8_t 1 if it is a leap year, 0 otherwise
 */
uint8_t tiny_isLeapYear(const uint16_t year);

/**
 * @brief Get the current month days of the year and month.
 *
 * The year is needed in case of a leap year for february
 *
 * @param year The current year
 * @param month The desired month
 * @return uint8_t The number of days in the month of the year
 */
uint8_t tiny_getMonthDays(const uint16_t year, const uint8_t month);

/**
 * @brief Convert the given seconds to the desired format.
 *
 * The passed arguments (!= NULL) will be included in the calculation.
 * Decide by passing only the interesting format as function arguments.
 *
 * Example: Pass only days if you need the seconds in days.
 * Pass hours and minutes if both are interesting.
 * THe result will be the hours, the minutes (0-59) and returns the remaining seconds.
 *
 * The remaining seconds will always be returned.
 *
 * @param seconds The seconds to convert
 * @param days The seconds in days as reference if needed, NULL otherwise
 * @param hours The seconds in hours as reference if needed, NULL otherwise
 * @param mins The seconds in minutes as reference if needed, NULL otherwise
 * @return uint64_t The remaining seconds. The passed seconds in case of non interested conversion
 */
uint64_t tiny_convertSeconds(const uint64_t seconds, uint64_t *days, uint64_t *hours, uint64_t *mins);

#ifdef __cplusplus
}
#endif

#endif /* TINY_TIME_H*/
