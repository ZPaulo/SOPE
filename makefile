primes: Sope.c Queue.c
	mkdir -p bin
	gcc Sope.c -lrt -lpthread -lm -Wall -o bin/primes
