CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror -Wtype-limits --coverage
LDFLAGS= \
-I../unity\
-I../inc

SRC=../src/tinytime.c ../unity/unity.c
TEST=test_tinyTimeLib.c
OUT=test_tinyTimeLib

all: build

build:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUT) $(SRC) $(TEST)

test: build
	./$(OUT)

coverage: test
	lcov --capture --directory . --output-file coverage.info
	lcov --ignore-errors unused --remove coverage.info '/tests/*' '/unity/*' --output-file coverage_filtered.info
	genhtml coverage_filtered.info --output-directory coverage_report

clean:
	rm -f $(OUT) *.gcda *.gcno *.info
	rm -rf coverage_report