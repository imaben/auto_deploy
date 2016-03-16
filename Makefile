# Makefile for Auto Deploy
C = gcc
CFLAGS = -g -c
LDFLAGS = -levent -lhiredis

all: main.c
	$(CC) -o daemonize.o daemonize.c $(CFLAGS)
	$(CC) -o worker.o worker.c $(CFLAGS)
	$(CC) -o main.o main.c $(CFLAGS)
	$(CC) -o deploy main.o daemonize.o worker.o $(LDFLAGS)

clean:
	rm -rf *.o
	rm -rf deploy
