/**
 * @file tinytime.c
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

#include "tinytime.h"
#include <stddef.h>
#include <stdio.h>

#define LEAP_YEAR_FREQUENCY (4)    ///< Frequency of a leap year
#define LEAP_YEAR_REMOVED (100)    ///< Removed leap year every century
#define LEAP_YEAR_CORRECTION (400) ///< Not removed leap year every 4 centuries
#define MONTH_DAY_OFFSET (1)       ///< Month day starts at 1

/**
 * @brief Checks if the check value is in range from min and max
 *
 */
#define IS_NOT_IN_RANGE(CHECK, MIN, MAX) ((CHECK) < (MIN) || (CHECK) > (MAX))

tinyUnixType tiny_getUnixTime(const tinyTimeType *tm) {
#define CENTURY_CORRECTION_OFFSET (1900)
#define FOUR_CENTURY_CORRECTION_OFFSET (1600)
#define FIRST_LEAP_YEAR_OFFSET (1)
#define ERROR_VALUE (0)

  if (NULL == tm) {
    return ERROR_VALUE;
  }
  // Check valid time
  if (IS_NOT_IN_RANGE(tm->sec, 0, TINY_SEC_MAX) ||
      IS_NOT_IN_RANGE(tm->min, 0, TINY_MINUTE_MAX) ||
      IS_NOT_IN_RANGE(tm->hour, 0, TINY_HOUR_MAX)) {
    return ERROR_VALUE;
  }
  // Check valid year
  if (IS_NOT_IN_RANGE(tm->year, TINY_UNIX_YEAR_BEGIN, UINT16_MAX)) {
    return ERROR_VALUE;
  }
  // Checks a valid day and month
  if (IS_NOT_IN_RANGE(tm->monthDay, MONTH_DAY_OFFSET, tiny_getMonthDays(tm->year, tm->month))) {
    return ERROR_VALUE;
  }

  const uint16_t unixYearDiff = tm->year - TINY_UNIX_YEAR_BEGIN;
  const uint16_t leapYearCheck = tm->year - FIRST_LEAP_YEAR_OFFSET; // Used to include last year, this year will be included in
  uint16_t numberOfLeapYears = (unixYearDiff + FIRST_LEAP_YEAR_OFFSET) / LEAP_YEAR_FREQUENCY - (leapYearCheck - CENTURY_CORRECTION_OFFSET) / LEAP_YEAR_REMOVED + (leapYearCheck - FOUR_CENTURY_CORRECTION_OFFSET) / LEAP_YEAR_CORRECTION;

  tinyUnixType days = (tinyUnixType)unixYearDiff * TINY_ONE_YEAR_IN_DAYS + numberOfLeapYears;

  for (uint8_t i = TINY_JAN; i < tm->month; i++) {
    days += (tinyUnixType)tiny_getMonthDays(tm->year, i);
  }

  days += (tinyUnixType)tm->monthDay - MONTH_DAY_OFFSET;
  return days * TINY_ONE_DAY_IN_SEC + tm->hour * TINY_ONE_HOUR_IN_SEC + tm->min * TINY_ONE_MIN_IN_SEC + tm->sec;
}

void tiny_getTimeType(tinyTimeType *tm, const tinyUnixType unixTime) {
  if (NULL == tm) {
    return;
  }
  // Get base values
  uint64_t days = unixTime / TINY_ONE_DAY_IN_SEC + 1;
  uint64_t secInDay = unixTime % TINY_ONE_DAY_IN_SEC;
  // Set current daytime
  tm->hour = (uint8_t)(secInDay / TINY_ONE_HOUR_IN_SEC);
  tm->min = (uint8_t)((secInDay % TINY_ONE_HOUR_IN_SEC) / TINY_ONE_MIN_IN_SEC);
  tm->sec = (uint8_t)(secInDay % TINY_ONE_MIN_IN_SEC);
  // Calculate Weekday, First day was a thursday (1.1.1970)
  tm->weakDay = (days + TINY_WED) % TINY_MAX_WEAKDAYS;
  // Get current year, move throw all years
  uint16_t year = TINY_UNIX_YEAR_BEGIN;
  while (1) {
    uint64_t daysInYear = TINY_ONE_YEAR_IN_DAYS + tiny_isLeapYear(year);
    if (days < daysInYear) {
      break;
    }
    days -= daysInYear; // Decrement the current year days
    year++;             // Increment the year
  }
  // Set year and the current yearDay as rest of current year
  tm->year = year;
  tm->yearDay = (uint16_t)days;
  // Get month and day
  for (uint8_t month = TINY_JAN; month < TINY_MAX_MONTHS; month++) {
    uint16_t daysInMonth = tiny_getMonthDays(year, month);
    if (days <= daysInMonth) {
      tm->month = month;
      tm->monthDay = days; // Starting at 1
      return;
    }
    days -= daysInMonth; // Decrement current month
  }
  // Error
  return;
}

const char *tiny_getFormat(const tinyTimeType *tm) {
  if (NULL == tm) {
    return NULL;
  }

#define BUFFER_SIZE (26)
  static char formatBuffer[BUFFER_SIZE];
  const char *weekDays[TINY_MAX_WEAKDAYS] = {
      [TINY_SUN] = "Sun",
      [TINY_MON] = "Mon",
      [TINY_TUE] = "Tue",
      [TINY_WED] = "Wed",
      [TINY_THU] = "Thu",
      [TINY_FRI] = "Fri",
      [TINY_SAT] = "Sat"};

  const char *months[TINY_MAX_MONTHS - TINY_JAN] = {
      [TINY_JAN -
          TINY_JAN] = "Jan",
      [TINY_FEB -
          TINY_JAN] = "Feb",
      [TINY_MAR -
          TINY_JAN] = "Mar",
      [TINY_APR -
          TINY_JAN] = "Apr",
      [TINY_MAY -
          TINY_JAN] = "May",
      [TINY_JUN -
          TINY_JAN] = "Jun",
      [TINY_JUL -
          TINY_JAN] = "Jul",
      [TINY_AUG -
          TINY_JAN] = "Aug",
      [TINY_SEP -
          TINY_JAN] = "Sep",
      [TINY_OCT -
          TINY_JAN] = "Oct",
      [TINY_NOV -
          TINY_JAN] = "Nov",
      [TINY_DEC -
          TINY_JAN] = "Dec"};
  if (IS_NOT_IN_RANGE(tm->weakDay, 0, TINY_SAT)) {
    snprintf(formatBuffer, BUFFER_SIZE, "Day %3d not in range", tm->weakDay);
  } else if (IS_NOT_IN_RANGE(tm->month), TINY_JAN, TINY_DEC) {
    snprintf(formatBuffer, BUFFER_SIZE, "Month %3d not in range", tm->month);
  } else {
    snprintf(formatBuffer, BUFFER_SIZE, "%s %2d %s %4d %.2d:%.2d:%.2d",
        weekDays[tm->weakDay], tm->monthDay, months[tm->month - 1], tm->year, tm->hour, tm->min, tm->sec);
  }
  return formatBuffer;
}

uint8_t tiny_isLeapYear(const uint16_t year) {
  return (0 == (year % LEAP_YEAR_FREQUENCY) && (year % LEAP_YEAR_REMOVED != 0 || 0 == (year) % LEAP_YEAR_CORRECTION));
}

uint8_t tiny_getMonthDays(const uint16_t year, const uint8_t month) {
  if (IS_NOT_IN_RANGE(month, TINY_JAN, TINY_DEC)) {
    return 0;
  }
  static const uint8_t daysPerMonth[TINY_MAX_MONTHS - TINY_JAN] = {
      [TINY_JAN -
          TINY_JAN] = 31,
      [TINY_FEB -
          TINY_JAN] = 28,
      [TINY_MAR -
          TINY_JAN] = 31,
      [TINY_APR -
          TINY_JAN] = 30,
      [TINY_MAY -
          TINY_JAN] = 31,
      [TINY_JUN -
          TINY_JAN] = 30,
      [TINY_JUL -
          TINY_JAN] = 31,
      [TINY_AUG -
          TINY_JAN] = 31,
      [TINY_SEP -
          TINY_JAN] = 30,
      [TINY_OCT -
          TINY_JAN] = 31,
      [TINY_NOV -
          TINY_JAN] = 30,
      [TINY_DEC -
          TINY_JAN] = 31};
  if (month == TINY_FEB) {
    // Check leap year and return
    return daysPerMonth[month - TINY_JAN] + tiny_isLeapYear(year);
  }
  return daysPerMonth[month - TINY_JAN];
}