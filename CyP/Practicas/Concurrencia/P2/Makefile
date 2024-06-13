CFLAGS=-g -Wall
OBJS=compress.o chunk_archive.o options.o queue.o comp.o
LIBS=-lz
CC=gcc

all: comp queuetest

comp: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

queuetest: mainqueue.o queue.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f *.o comp queuetest
