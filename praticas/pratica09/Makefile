CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

main: main.c cobertura.c cobertura.h
	$(CC) $(CFLAGS) main.c cobertura.c -o main

clean:
	rm -f main main.exe *.o

.PHONY: clean
