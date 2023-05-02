CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic

advcalc2ir: main.c
	$(CC) $(CFLAGS) -o advcalc2ir main.c
