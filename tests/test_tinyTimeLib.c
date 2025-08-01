/**
 * @file test_tinyTimeLib.c
 * @author Adrian STEINER (steia19@bfh.ch)
 * @brief test tinytime lib with https://github.com/ThrowTheSwitch/Unity tests
 * @version 0.1
 * @date 21-03-2025
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
#include "unity.h"

#include <stdbool.h>
#include <string.h>

/* timeType and Unix time base */
struct
{
  tinyTimeType timeType;
  tinyUnixType unixTime;
  char formatString[26];
} testTimes[] = {
    {.timeType = {0},
        .unixTime = UINT64_MAX,
        .formatString = ""},
    {.timeType = {
         .sec = 1,
         .min = 0,
         .hour = 0,
         .monthDay = 1,
         .month = TINY_JAN,
         .year = 1970,
         .weakDay = TINY_THU,
         .yearDay = 1,
     },
        .unixTime = 1,
        .formatString = "Thu  1 Jan 1970 00:00:01"},
    {.timeType = {
         .sec = 56,
         .min = 34,
         .hour = 12,
         .monthDay = 21,
         .month = TINY_MAR,
         .year = 2025,
         .weakDay = TINY_FRI,
         .yearDay = 80,
     },
        .unixTime = 1742560496,
        .formatString = "Fri 21 Mar 2025 12:34:56"},
    {.timeType = {
         .sec = 59,
         .min = 0,
         .hour = 0,
         .monthDay = 1,
         .month = TINY_JAN,
         .year = 2000,
         .weakDay = TINY_SAT,
         .yearDay = 1,
     },
        .unixTime = 946684859,
        .formatString = "Sat  1 Jan 2000 00:00:59"},
    {.timeType = {
         .sec = 30,
         .min = 45,
         .hour = 23,
         .monthDay = 29,
         .month = TINY_FEB,
         .year = 2024,
         .weakDay = TINY_THU,
         .yearDay = 60,
     },
        .unixTime = 1709250330,
        .formatString = "Thu 29 Feb 2024 23:45:30"},
    {.timeType = {
         .sec = 15,
         .min = 30,
         .hour = 18,
         .monthDay = 15,
         .month = TINY_JUL,
         .year = 2010,
         .weakDay = TINY_THU,
         .yearDay = 196,
     },
        .unixTime = 1279218615,
        .formatString = "Thu 15 Jul 2010 18:30:15"},
    {.timeType = {
         .sec = 17,
         .min = 16,
         .hour = 8,
         .monthDay = 3,
         .month = TINY_NOV,
         .year = 1995,
         .weakDay = TINY_FRI,
         .yearDay = 307,
     },
        .unixTime = 815386577,
        .formatString = "Fri  3 Nov 1995 08:16:17"},
    {.timeType = {
         .sec = 17,
         .min = 16,
         .hour = 15,
         .monthDay = 12,
         .month = TINY_DEC,
         .year = 2123,
         .weakDay = TINY_SUN,
         .yearDay = 346,
     },
        .unixTime = 4858067777,
        .formatString = "Sun 12 Dec 2123 15:16:17"}};

void setUp(void) {
} // Empty needed definition
void tearDown(void) {
} // Empty needed definition

void compareTimeTypes(const tinyTimeType *expected, const tinyTimeType *actual) {
  TEST_ASSERT_EQUAL_UINT8(expected->sec, actual->sec);
  TEST_ASSERT_EQUAL_UINT8(expected->min, actual->min);
  TEST_ASSERT_EQUAL_UINT8(expected->hour, actual->hour);

  TEST_ASSERT_EQUAL_UINT8(expected->monthDay, actual->monthDay);
  TEST_ASSERT_EQUAL_UINT8(expected->month, actual->month);
  TEST_ASSERT_EQUAL_UINT8(expected->year, actual->year);

  TEST_ASSERT_EQUAL_UINT8(expected->weakDay, actual->weakDay);
  TEST_ASSERT_EQUAL_UINT8(expected->yearDay, actual->yearDay);
}

void test_isLeapYear(void) {
  uint16_t leapYears[] = {1904, 1908, 1912, 1916, 1920, 1924, 1928, 1932, 1936, 1940, 1944, 1948, 1952, 1956, 1960, 1964, 1968, 1972, 1976, 1980, 1984, 1988, 1992, 1996, 2000, 2004, 2008, 2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040, 2044, 2048, 2052, 2056, 2060, 2064, 2068, 2072, 2076, 2080, 2084, 2088, 2092, 2096, 2104, 2108, 2112, 2116, 2120, 2124, 2128, 2132, 2136, 2140, 2144, 2148, 2152, 2156, 2160, 2164, 2168, 2172, 2176, 2180, 2184, 2188, 2192, 2196, 2400, 2800};
  for (uint16_t i = 0; i < sizeof(leapYears) / sizeof(leapYears[0]); i++) {
    TEST_ASSERT_EQUAL_UINT8(1, tiny_isLeapYear(leapYears[i]));
  }
}

void test_getMonthDays(void) {
#define NUMBER_OF_TESTS (15)
  uint8_t expectedValues[NUMBER_OF_TESTS][2] = {
      {0, 0},
      {31, TINY_JAN},
      {28, TINY_FEB},
      {31, TINY_MAR},
      {30, TINY_APR},
      {31, TINY_MAY},
      {30, TINY_JUN},
      {31, TINY_JUL},
      {31, TINY_AUG},
      {30, TINY_SEP},
      {31, TINY_OCT},
      {30, TINY_NOV},
      {31, TINY_DEC},
      {0, TINY_MAX_MONTHS},
      {0, UINT8_MAX},
  };
  for (uint16_t i = 0; i < NUMBER_OF_TESTS; i++) {
    TEST_ASSERT_EQUAL_UINT8(expectedValues[i][0], tiny_getMonthDays(2001, expectedValues[i][1])); // No leap year
  }
  // Test leap year with february
  TEST_ASSERT_EQUAL_UINT8(29, tiny_getMonthDays(2000, TINY_FEB)); // Leap year
}

void test_getUnixTime(void) {
  // Check valid dates
  for (size_t i = 0; i < sizeof(testTimes) / sizeof(testTimes[0]); i++) {
    TEST_ASSERT_EQUAL_UINT64(testTimes[i].unixTime, tiny_getUnixTime(&testTimes[i].timeType));
  }
  // Check invalid dates and times
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX, tiny_getUnixTime(NULL));
  tinyTimeType invalidTimeType = {0};
  invalidTimeType.sec = TINY_SEC_MAX + 1;
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX, tiny_getUnixTime(&invalidTimeType));
  invalidTimeType.sec = TINY_SEC_MAX;
  invalidTimeType.year = TINY_UNIX_YEAR_BEGIN - 1;
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX, tiny_getUnixTime(&invalidTimeType));
  invalidTimeType.year = TINY_UNIX_YEAR_BEGIN;
  invalidTimeType.month = TINY_MAX_MONTHS;
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX, tiny_getUnixTime(&invalidTimeType));
}

void test_getTimeType(void) {
  tinyTimeType testDate = {0};
  // Add NULL test
  tiny_getTimeType(NULL, 0);
  // Check valid dates
  for (size_t i = 1; i < sizeof(testTimes) / sizeof(testTimes[0]); i++) {
    tiny_getTimeType(&testDate, testTimes[i].unixTime);
    compareTimeTypes(&testTimes[i].timeType, &testDate);
  }
}

void test_getFormat(void) {
  // Check NULL argument
  TEST_ASSERT_EQUAL_PTR(NULL, tiny_getFormat(NULL));
  // Check valid days
  for (size_t i = 1; i < sizeof(testTimes) / sizeof(testTimes[0]); i++) {
    TEST_ASSERT_EQUAL_STRING(testTimes[i].formatString, tiny_getFormat(&testTimes[i].timeType));
  }
  tinyTimeType wrongValues = {
      .weakDay = TINY_MAX_WEAKDAYS,
      .month = TINY_JAN};
  TEST_ASSERT_EQUAL_STRING("Day   7 not in range", tiny_getFormat(&wrongValues));
  wrongValues.weakDay = TINY_SUN,
  wrongValues.month = TINY_MAX_MONTHS;
  TEST_ASSERT_EQUAL_STRING("Month  13 not in range", tiny_getFormat(&wrongValues));
}

void test_convertSeconds(void) {

  typedef struct {
    uint64_t inputSeconds;
    bool use_days;
    bool use_hours;
    bool use_mins;
    uint64_t expectedDays;
    uint64_t expectedHours;
    uint64_t expectedMins;
    uint64_t expectedRemaining;
  } convertSeconds_testCase;

  convertSeconds_testCase tests[] = {
      {987654, true, true, true, 11, 10, 20, 54},
      {987654, false, true, true, 0, 274, 20, 54},
      {987654, false, false, true, 0, 0, 16460, 54},
      {987654, false, false, false, 0, 0, 0, 987654},
      {86400, true, true, true, 1, 0, 0, 0},
      {3661, false, true, true, 0, 1, 1, 1},
      {59, true, true, true, 0, 0, 0, 59},
      {3600, false, true, false, 0, 1, 0, 0},
      {60, false, false, true, 0, 0, 1, 0},
      {0, true, true, true, 0, 0, 0, 0}};

  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    convertSeconds_testCase *test = &tests[i];
    uint64_t days = 0, hours = 0, mins = 0;
    uint64_t *p_days = test->use_days ? &days : NULL;
    uint64_t *p_hours = test->use_hours ? &hours : NULL;
    uint64_t *p_mins = test->use_mins ? &mins : NULL;
    TEST_ASSERT_EQUAL_UINT64(test->expectedRemaining, tiny_convertSeconds(test->inputSeconds, p_days, p_hours, p_mins));

    if (p_days) {
      TEST_ASSERT_EQUAL_UINT64(test->expectedDays, *p_days);
    }
    if (p_hours) {
      TEST_ASSERT_EQUAL_UINT64(test->expectedHours, *p_hours);
    }
    if (p_mins) {
      TEST_ASSERT_EQUAL_UINT64(test->expectedMins, *p_mins);
    }
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_isLeapYear);
  RUN_TEST(test_getMonthDays);
  RUN_TEST(test_getUnixTime);
  RUN_TEST(test_getTimeType);
  RUN_TEST(test_getFormat);
  RUN_TEST(test_convertSeconds);
  return UNITY_END();
}