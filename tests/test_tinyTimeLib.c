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

void setUp(void) {
} // Empty needed definition
void tearDown(void) {
} // Empty needed definition

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

void test__getUnixTime(void) {
  tinyTimeType testDate = {0};
  TEST_ASSERT_EQUAL_UINT64(0, tiny_getUnixTime(&testDate));
  testDate.month = TINY_JAN;
  testDate.year = 1970;
  testDate.monthDay = 1;
  TEST_ASSERT_EQUAL_UINT64(0, tiny_getUnixTime(&testDate));

  testDate.sec = 1;
  testDate.month = TINY_JAN;
  testDate.year = 1970;
  testDate.monthDay = 1;
  TEST_ASSERT_EQUAL_UINT64(1, tiny_getUnixTime(&testDate));

  testDate.sec = 56;
  testDate.min = 34;
  testDate.hour = 12;
  testDate.monthDay = 21;
  testDate.month = TINY_MAR;
  testDate.year = 2025;
  TEST_ASSERT_EQUAL_UINT64(1742560496, tiny_getUnixTime(&testDate));

  testDate.sec = 59;
  testDate.min = 0;
  testDate.hour = 0;
  testDate.monthDay = 1;
  testDate.month = TINY_JAN;
  testDate.year = 2000;
  TEST_ASSERT_EQUAL_UINT64(946684859, tiny_getUnixTime(&testDate));

  testDate.sec = 30;
  testDate.min = 45;
  testDate.hour = 23;
  testDate.monthDay = 29;
  testDate.month = TINY_FEB;
  testDate.year = 2024;
  TEST_ASSERT_EQUAL_UINT64(1709250330, tiny_getUnixTime(&testDate));

  testDate.sec = 15;
  testDate.min = 30;
  testDate.hour = 18;
  testDate.monthDay = 15;
  testDate.month = TINY_JUL;
  testDate.year = 2010;
  TEST_ASSERT_EQUAL_UINT64(1279218615, tiny_getUnixTime(&testDate));

  testDate.sec = 17;
  testDate.min = 16;
  testDate.hour = 8;
  testDate.monthDay = 3;
  testDate.month = TINY_NOV;
  testDate.year = 1995;
  TEST_ASSERT_EQUAL_UINT64(815386577, tiny_getUnixTime(&testDate));

  testDate.sec = 17;
  testDate.min = 16;
  testDate.hour = 15;
  testDate.monthDay = 12;
  testDate.month = TINY_DEC;
  testDate.year = 2123;
  TEST_ASSERT_EQUAL_UINT64(4858067777, tiny_getUnixTime(&testDate));
}

void test_getTimeType(void) {
}

void test_getFormat(void);

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_isLeapYear);
  RUN_TEST(test_getMonthDays);
  RUN_TEST(test__getUnixTime);
  return UNITY_END();
}