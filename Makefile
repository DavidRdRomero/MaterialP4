CC=gcc
CFLAGS=-Wall -g -DDEBUG
CLIB= -L. -lm

all: p4

p4: p4.c bstree.h maze.h
	$(CC) $(CFLAGS) -o $@ $< maze.c bstree.c $(CLIB)

##############################################
clean:
	@echo "Limpiando..."
	@rm -f p4	