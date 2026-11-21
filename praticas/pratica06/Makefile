CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

main: main.c dijkstra.c dijkstra.h
	$(CC) $(CFLAGS) main.c dijkstra.c -o main

clean:
	rm -f main main.exe *.o

.PHONY: clean
