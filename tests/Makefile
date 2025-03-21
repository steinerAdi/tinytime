CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic
LDFLAGS= \
-I../unity\
-I../lib

SRC=../lib/tinytime.c ../unity/unity.c
TEST=test_tinyTimeLib.c
OUT=test_tinyTimeLib

all: build

build:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUT) $(SRC) $(TEST)

test: build
	./$(OUT)

clean:
	rm -f $(OUT)