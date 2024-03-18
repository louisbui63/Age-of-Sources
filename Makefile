CC = clang

CFLAGS = -std=gnu2x -Wall -Wextra -pedantic -O2 -fopenmp=libomp #-Isrc

LIBS = $(shell pkg-config --cflags --libs sdl2)

CFLAGS += -g -fno-omit-frame-pointer #-fsanitize=address

SRC=$(subst src,build/src,$(subst .c,.o,$(shell find src/ -type f -name '*.c')))
TEST_SRC=$(subst tests/,build/tests/,$(subst .c,.o,$(wildcard tests/*.c)))

.PHONY: all test format clean run test_run doc

all: $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -o main $(SRC)

build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $(LIBS) -o test $(subst build/src/main.o,,$(SRC)) $(TEST_SRC)

format:
	./format.sh

doc:
	doxygen Doxyfile
	make -C ./latex

clean:
	rm -rf build/*
	rm -f main test

run: all
	./main

test_run: test
	valgrind ./test
