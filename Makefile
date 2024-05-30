CC = clang

CFLAGS = -std=gnu2x -Wall -Wextra -pedantic -O2 -fopenmp=libomp  -Wno-gnu-empty-struct

LIBS = $(shell pkg-config --cflags --libs sdl2 SDL2_mixer SDL2_ttf) -lm

CFLAGS += -g -fno-omit-frame-pointer #-fsanitize=address

SRC=$(subst src,build/src,$(subst .c,.o,$(shell find src/ -type f -name '*.c')))
TEST_SRC=$(subst tests/,build/tests/,$(subst .c,.o,$(wildcard tests/*.c)))

.PHONY: all test format clean run test_run doc

all: $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -o main $(SRC)

build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

add_flag:
	$(eval CFLAGS += -DIS_TEST)

test: clean add_flag $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $(LIBS) -o test $(subst build/src/main.o,,$(SRC)) $(TEST_SRC)

format:
	./format.sh

doc:
	make -C ./doc doc

latex:
	make -C ./doc latex

htmldoc :
	firefox https://uwu-segfault.eu/2p2doc/

clean:
	rm -rf build/*
	rm -f main test

run: all
	./main

test_run: test
	valgrind ./test
