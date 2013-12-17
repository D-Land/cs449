CC=gcc
CFLAGS=-m32 -c

driver: my_malloc.o mallocdrv.o
	$(CC) -m32 -o driver my_malloc.o mallocdrv.o

my_malloc.o: my_malloc.c my_malloc.h
	$(CC) $(CFLAGS) -o my_malloc.o my_malloc.c

mallocdrv.o: mallocdrv.c my_malloc.h
	$(CC) $(CFLAGS) -o mallocdrv.o mallocdrv.c
