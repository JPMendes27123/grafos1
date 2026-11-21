CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

main: main.c hamiltoniano.c hamiltoniano.h tps.c tps.h
	$(CC) $(CFLAGS) main.c hamiltoniano.c tps.c -o main

clean:
	rm -f main main.exe *.o

.PHONY: clean
