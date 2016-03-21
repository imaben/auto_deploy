# Makefile for Auto Deploy
C = gcc
CFLAGS = -g -c `pkg-config --cflags libevent` `pkg-config --cflags hiredis`
LDFLAGS = -lm -lpthread `pkg-config --libs libevent` `pkg-config --libs hiredis`

OS_KERNEL = $(shell uname -s)
ifeq ($(OS_KERNEL), Darwin)
MICRO =	 -D __OSX__
else
MICRO =	 -D __LINUX__
LDFLAGS += -lutil
endif


all: main.c
	$(CC) -o daemonize.o daemonize.c $(CFLAGS)
	$(CC) -o worker.o worker.c $(CFLAGS)
	$(CC) -o redis.o redis.c $(CFLAGS)
	$(CC) -o cJSON.o cJSON.c $(CFLAGS)
	$(CC) -o process.o process.c $(CFLAGS) $(MICRO)
	$(CC) -o utils.o utils.c $(CFLAGS)
	$(CC) -o main.o main.c $(CFLAGS)
	$(CC) -o deploy main.o daemonize.o worker.o redis.o cJSON.o process.o utils.o $(LDFLAGS)

clean:
	rm -rf *.o
	rm -rf deploy
