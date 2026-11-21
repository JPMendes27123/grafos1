CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

main: main.c agm.c agm.h
	$(CC) $(CFLAGS) main.c agm.c -o main

clean:
	rm -f main main.exe *.o

.PHONY: clean
