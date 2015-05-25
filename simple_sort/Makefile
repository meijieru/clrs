CC = gcc
CFLAGS = -O1 -g
VPATH = heap_sort:insertion_sort:merge_sort

all:Heap_sort.o Merge_sort.o Insertion_sort.o sort.o
	(cd heap_sort; make all)
	(cd merge_sort; make all)
	(cd insertion_sort; make all)
	$(CC) $(CFLAGS) -o sort.o Heap_sort.o Merge_sort.o Insertion_sort.o

Insertion_sort.o:Insertion_sort.c Insertion_sort.h
	gcc -c -O1 Insertion_sort.c


sort.o:sort.c sort.h
	$(CC) $(CFLAGS) -c sort.c

clean:
	rm -f sort
	(cd heap_sort; make clean)
	(cd merge_sort; make clean)
	(cd insertion_sort; make clean)
