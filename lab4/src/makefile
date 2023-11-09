CC=gcc
CFLAGS=-I.

all : parallel_min_max create_zombie

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

create_zombie :
	$(CC) -o create_zombie zombi.c $(CFLAGS)

process_memory : 
	$(CC) -o process_memory process_memory.c $(CFLAGS)

sum : sum.h
	$(CC) -o sum.o -c sum.c -pthread $(CFLAGS)

parallel_sum : utils.h utils.o sum.o
	$(CC) -o parallel_sum utils.o  parallel_sum.c -pthread $(CFLAGS)

clean_min_max :
	rm utils.o find_min_max.o parallel_min_max

clean_zombie : 
	rm create_zombie

clean_process :
	rm process_memory

clean_sum : 
	rm utils.o sum.o parallel_sum

psum : utils.o  # parallel_sum с статической библиотекой sum.a
	$(CC) -c sum.c -o bin/sum.o
	$(CC) -c parallel_sum.c -o parallel_sum.o -pthread
	ar rcs bin/libsum.a bin/sum.o
	$(CC) parallel_sum.o  -Lbin -lsum -o parallel_sum -pthread

clean_psum :
	rm bin/sum.o bin/libsum.a parallel_sum parallel_sum.o utils.o
