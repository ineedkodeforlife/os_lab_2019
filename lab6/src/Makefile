CC=gcc
CFLAGS=-I. 

objects = server client start_servers

all: $(objects)

server : server.o
	$(CC) server.o -Lbin -lmult -o server -pthread $(CFLAGS)

client: client.o
	$(CC) client.o -Lbin -lmult -o client -pthread $(CFALGS)

start_servers:
	./server --port 20001 --tnum 5 &
	./server --port 20002 --tnum 5 &
start:
	./client --k 10 --mod 19 --servers /serv

server.o : 
	$(CC) -o server.o -c server.c $(CFLAGS)

client.o : 
	$(CC) -o client.o -c client.c $(CFLAGS)

clean :
	rm client server client.o server.o
	fuser -k 20001/tcp
	fuser -k 20002/tcp
