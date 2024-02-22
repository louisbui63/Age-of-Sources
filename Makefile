CFLAGS = -std=c2x -Wall -Wextra -pedantic
CC = clang

SRC=$(subst .c,.o,$(wildcard *.c))
TEST_SRC=$(subst .c,.o,$(wildcard tests/*.c))

all: $(SRC)
	$(CC) $(CFLAGS) -lSDL2 -o main *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_SRC) $(SRC)
	$(CC) $(CFLAGS) -o test $(subst main.o,,$(wildcard *.o)) tests/*.o

clean:
	rm -f **/*.o main test

run: all
	./main

test_run: test
	./test
