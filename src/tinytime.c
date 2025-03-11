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

#define LEAP_YEAR_FREQUENCY (4)
#define LEAP_YEAR_REMOVED (100)
#define LEAP_YEAR_CORRECTION (400)

tinyUnixType tiny_getUnixTime(const tinyTimeType *tm) {
  if (NULL == tm) {
    return 0;
  }
#define CORRECTION_OFFSET (1600)
  const uint16_t currentYear = tm->year + TINY_YEAR_OFFSET;
  const uint16_t unixYearDiff = currentYear - TINY_UNIX_YEAR_BEGIN;
  uint16_t leap_years = (unixYearDiff + 2) / LEAP_YEAR_FREQUENCY - (tm->year) / LEAP_YEAR_REMOVED + (currentYear - CORRECTION_OFFSET) / LEAP_YEAR_CORRECTION;

  tinyUnixType days = (tinyUnixType)unixYearDiff * TINY_ONE_YEAR_IN_DAYS + leap_years;

  for (uint8_t i = 0; i < tm->month; i++) {
    days += (tinyUnixType)tiny_getMonthDays(tm->year, i);
  }

  days += (tinyUnixType)tm->monthDay - 1;
  return days * TINY_ONE_DAY_IN_SEC + tm->hour * TINY_ONE_HOUR_IN_SEC + tm->min * TINY_ONE_MIN_IN_SEC + tm->sec;
}

void tiny_getTimeType(tinyTimeType *tm, const tinyUnixType unixTime) {
  if (NULL == tm) {
    return;
  }
  // Get base values
  uint64_t days = unixTime / TINY_ONE_DAY_IN_SEC;
  uint64_t secInDay = unixTime % TINY_ONE_DAY_IN_SEC;
  // Set current daytime
  tm->hour = (uint8_t)(secInDay / TINY_ONE_HOUR_IN_SEC);
  tm->min = (uint8_t)((secInDay % TINY_ONE_HOUR_IN_SEC) / TINY_ONE_MIN_IN_SEC);
  tm->sec = (uint8_t)(secInDay % TINY_ONE_MIN_IN_SEC);
  // Calculate Weekday, First day was a thursday (1.1.1970)
  tm->weakDay = (days + TINY_THU) % TINY_MAX_WEAKDAYS;
  // Get current year, move throw all years
  uint16_t year = TINY_UNIX_YEAR_BEGIN;
  while (1) {
    uint64_t daysInYear = TINY_ONE_YEAR_IN_DAYS + tiny_isLeapYear(year);
    if (days < daysInYear) { //
      break;
    }
    days -= daysInYear; // Decrement the current year days
    year++;             // Increment the year
  }
  // Set year and the current yearDay as rest of current year
  tm->year = year - TINY_YEAR_OFFSET;
  tm->yearDay = (uint16_t)days;
  // Get month and day
  for (uint8_t month = 0; month < TINY_MAX_MONTHS; month++) {
    uint16_t daysInMonth = tiny_getMonthDays(year, month);
    if (days < daysInMonth) {
      tm->month = month;
      tm->monthDay = days + 1; // Starting at 1
      return;
    }
    days -= daysInMonth; // Decrement current month
  }
  // Error
  return;
}

void tiny_getFormat(const tinyTimeType *tm, char *buf, uint32_t bufSize) {
  if (NULL == tm || NULL == buf || 0 == bufSize) {
    return;
  }
#define BUFFER_SIZE (26)
  static const char *weekDays[TINY_MAX_WEAKDAYS] = {
      [TINY_SUN] = "Sun",
      [TINY_MON] = "Mon",
      [TINY_TUE] = "Tue",
      [TINY_WED] = "Wed",
      [TINY_THU] = "Thu",
      [TINY_FRI] = "Fri",
      [TINY_SAT] = "Sat"};

  static const char *months[TINY_MAX_MONTHS] = {
      [TINY_JAN] = "Jan",
      [TINY_FEB] = "Feb",
      [TINY_MAR] = "Mar",
      [TINY_APR] = "Apr",
      [TINY_MAY] = "May",
      [TINY_JUN] = "Jun",
      [TINY_JUL] = "Jul",
      [TINY_AUG] = "Aug",
      [TINY_SEP] = "Sep",
      [TINY_OCT] = "Oct",
      [TINY_NOV] = "Nov",
      [TINY_DEC] = "Dec"};

  snprintf(buf, bufSize, "%s %2d %s %4d %.2d:%.2d:%.2d",
      weekDays[tm->weakDay], tm->monthDay, months[tm->month], tm->year + TINY_YEAR_OFFSET, tm->hour, tm->min, tm->sec);
}

uint8_t tiny_isLeapYear(const uint16_t year) {
  return (0 == (year % LEAP_YEAR_FREQUENCY) && (year % LEAP_YEAR_REMOVED != 0 || 0 == (year) % LEAP_YEAR_CORRECTION));
}

uint8_t tiny_getMonthDays(const uint16_t year, const uint8_t month) {
  static const uint8_t daysPerMonth[TINY_MAX_MONTHS] = {
      [TINY_JAN] = 31,
      [TINY_FEB] = 28,
      [TINY_MAR] = 31,
      [TINY_APR] = 30,
      [TINY_MAY] = 31,
      [TINY_JUN] = 30,
      [TINY_JUL] = 31,
      [TINY_AUG] = 31,
      [TINY_SEP] = 30,
      [TINY_OCT] = 31,
      [TINY_NOV] = 30,
      [TINY_DEC] = 31};
  if (month == TINY_FEB) {
    // Check leap year and return
    return daysPerMonth[month] + tiny_isLeapYear(year);
  }
  return daysPerMonth[month];
}