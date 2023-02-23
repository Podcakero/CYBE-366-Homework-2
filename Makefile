CC = gcc
CFLAGS = -g -Wall
RM = rm -f

default: all

all: homework_2.c
	$(CC) $(CFLAGS) -o homework_2 homework_2.c

clean:
	$(RM) homework_2 @~