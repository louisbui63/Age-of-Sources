CFLAGS = -std=c2x -Wall -Wextra -pedantic -O2 -g -fno-omit-frame-pointer #-fsanitize=address
CC = clang

SRC=$(subst .c,.o,$(wildcard *.c))
TEST_SRC=$(subst .c,.o,$(wildcard tests/*.c))

all: $(SRC)
	$(CC) $(CFLAGS) -lSDL2 -o main *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
test: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) -o test $(subst main.o,,$(SRC)) $(TEST_SRC)

clean:
	rm -f **/*.o *.o main test

run: all
	./main

test_run: test
	valgrind ./test
