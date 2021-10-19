TARGET=dra
OBJ=DRA.o thread_client.o thread_server.o headers.h
CC=gcc
CFLAG=-c

all:$(TARGET) 

$(TARGET): $(OBJ)
	$(CC) -o dra DRA.o thread_client.o thread_server.o headers.h -lpthread

DRA.o: DRA.c
	$(CC) $(CFLAG) $<

thread_client.o: thread_client.c 
	$(CC) $(CFLAG) $< 

thread_server.o: thread_server.c 
	$(CC) $(CFLAG) $< 



clean:
	rm -rf *.o dra
