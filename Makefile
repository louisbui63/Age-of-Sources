CC = clang++

ifeq ($(CC), g++)
CFLAGS = -std=gnu++2b -Wall -Wextra -pedantic -O2 -fopenmp
else # we assume it's clang
CFLAGS = -std=gnu++2b -Wall -Wextra -pedantic -O2 -fopenmp=libomp
endif

CFLAGS += $(shell pkg-config --cflags sdl2 SDL2_mixer SDL2_ttf)
CFLAGS += -g -fno-omit-frame-pointer #-fsanitize=address

LIBS = $(shell pkg-config --libs sdl2 SDL2_mixer SDL2_ttf) -lm

SRC=$(subst src,build/src,$(subst .cpp,.o,$(shell find src/ -type f -name '*.cpp')))
TEST_SRC=$(subst tests/,build/tests/,$(subst .cpp,.o,$(wildcard tests/*.cpp)))

.PHONY: all test format clean run test_run doc

all: $(SRC)
	$(CC) -o main $(SRC) $(CFLAGS) $(LIBS)

build/%.o: %.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

add_test_flag:
	$(eval CFLAGS += -DIS_TEST)

test: add_test_flag $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $(subst build/src/main.o,,$(SRC)) $(TEST_SRC) $(LIBS) -o test

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
