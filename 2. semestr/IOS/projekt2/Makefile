CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

proj2: proj2.c
	$(CC) $(CFLAGS) proj2.c -o proj2 -lpthread

clear:
	rm -f *.o proj2

zip: *.c  Makefile
	zip proj2.zip *.c Makefile