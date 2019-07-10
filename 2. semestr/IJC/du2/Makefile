CC=gcc
CFLAGS=-std=c99 -O2 -Wall -pedantic -Wextra
CPP=g++
CCFLAGS=-std=c++11 -pedantic -Wall

all: tail tail2

tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail

tail2: tail2.cc
	$(CPP) $(CCFLAGS) tail2.cc -o tail2

clear:
	rm -f *.o tail tail2 xvalec01.zip

zip: *.c *.cc Makefile
	zip xvalec01.zip *.c *.cc  Makefile