CC=gcc
CFLAGS=-std=c99 -O2  -Wall -pedantic

all: primes primes-i steg-decode

primes: primes.c error.c eratosthenes.c bit_array.h
	$(CC) $(CFLAGS) primes.c error.c eratosthenes.c bit_array.h -lm -o primes

primes-i: primes.c error.c eratosthenes.c bit_array.h
	$(CC) $(CFLAGS) -DUSE_INLINE primes.c error.c eratosthenes.c bit_array.h -lm -o primes-i

steg-decode: steg-decode.c error.c eratosthenes.c ppm.c bit_array.h
	$(CC) $(CFLAGS) steg-decode.c error.c eratosthenes.c ppm.c bit_array.h -lm -o steg-decode

clear:
	rm -f *.o primes primes-i steg-decode xvalec01.zip

run: all 
	time ./primes
	time ./primes-i

zip: *.c *.h Makefile
	zip xvalec01.zip *.c *.h Makefile
