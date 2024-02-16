CFLAGS = -std=c2x -Wall -Wextra -pedantic -lSDL2
CC = clang

SRC=$(subst .c,.o,$(wildcard *.c))

all: $(SRC)
	$(CC) $(CFLAGS) -o main *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


