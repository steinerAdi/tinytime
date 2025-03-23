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

#define TINY_SEC_MAX (59)    ///< Max second value
#define TINY_MINUTE_MAX (59) ///< Max minute value
#define TINY_HOUR_MAX (23)   ///< Max hour value

#define TINY_UNIX_YEAR_BEGIN (1970) ///< Starting year of Unix time epoch
#define TINY_ONE_MIN_IN_SEC (60)    ///< One minute in seconds
#define TINY_ONE_HOUR_IN_SEC (3600) ///< One hour in seconds
#define TINY_ONE_DAY_IN_SEC (86400) ///< One day in seconds
#define TINY_ONE_YEAR_IN_DAYS (365) ///< One basic year in days

tinyUnixType tiny_getUnixTime(const tinyTimeType *tm);

void tiny_getTimeType(tinyTimeType *tm, const tinyUnixType unixTime);

const char *tiny_getFormat(const tinyTimeType *tm);

uint8_t tiny_isLeapYear(const uint16_t year);

uint8_t tiny_getMonthDays(const uint16_t year, const uint8_t month);

#ifdef __cplusplus
}
#endif

#endif /* TINY_TIME_H*/
