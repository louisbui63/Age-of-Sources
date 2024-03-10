CC = clang

CFLAGS = -std=c2x -Wall -Wextra -pedantic -O2

LIBS = $(shell pkg-config --cflags --libs sdl2)

CFLAGS += -g -fno-omit-frame-pointer #-fsanitize=address

SRC=$(subst .c,.o,$(wildcard *.c))
TEST_SRC=$(subst .c,.o,$(wildcard tests/*.c))

.PHONY: all test format clean run test_run

all: $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -o main *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $(LIBS) -o test $(subst main.o,,$(SRC)) $(TEST_SRC)

format:
	./format.sh

clean:
	rm -f **/*.o *.o main test

run: all
	./main

test_run: test
	valgrind ./test
