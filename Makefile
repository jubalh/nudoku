CC = gcc
SOURCES = main.c sudoku.c
LDFLAGS = -lncurses
CFLAGS=
NAME := nudoku

.PHONY: all developer clean run

all: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $(NAME)

developer: CFLAGS += -Wall -Wextra -pedantic -DDEBUG
developer: all

clean:
	rm -f *.o $(NAME)

run: all
	./$(NAME)
