CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

main: main.c bellman_ford.c bellman_ford.h
	$(CC) $(CFLAGS) main.c bellman_ford.c -o main

clean:
	rm -f main main.exe *.o

.PHONY: clean
