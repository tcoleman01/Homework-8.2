CC=gcc
CFLAGS=-O2 -std=c11 -Wall -Wextra -pedantic

all: rbtree

rbtree: main.c rbtree.c rbtree.h
	$(CC) $(CFLAGS) -o rbtree main.c rbtree.c

clean:
	rm -f rbtree
