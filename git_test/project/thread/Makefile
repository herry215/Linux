
all:test

test: headers.h mgwServer.o
	gcc -o sub headers.h mgwServer.o -lpthread

mgwServer.o: mgwServer.c
	gcc -c mgwServer.c

clean:
	rm -rf *.o sub
