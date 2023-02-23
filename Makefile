CC = gcc
CFLAGS = -g -Wall
LIBS = -lssl -lcrypto -L/usr/lib
RM = rm -f

default: all

all: homework_2.c
	$(CC) $(CFLAGS) -o homework_2 homework_2.c $(LIBS)

clean:
	$(RM) homework_2 /~