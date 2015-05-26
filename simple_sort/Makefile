CC = gcc
CFLAGS = -O1 -g
VPATH = heap_sort:insertion_sort:merge_sort

all:Heap_sort.o Merge_sort.o Insertion_sort.o sort.o Quick_sort.o
	$(CC) $(CFLAGS) -o sort.o Heap_sort.o Merge_sort.o Insertion_sort.o Quick_sort.o

Insertion_sort.o:./insertion_sort/Insertion_sort.c ./insertion_sort/Insertion_sort.h
	$(CC) $(CFLAGS) -c ./insertion_sort/Insertion_sort.c

Quick_sort.o:./quick_sort/Quick_sort.c ./quick_sort/Quick_sort.h
	$(CC) $(CFLAGS) -c ./quick_sort/Quick_sort.c

Heap_sort.o:./heap_sort/Heap_sort.c ./heap_sort/Heap_sort.h
	$(CC) $(CFLAGS) -c ./heap_sort/Heap_sort.c

Merge_sort.o:./merge_sort/Merge_sort.c ./merge_sort/Merge_sort.h
	$(CC) $(CFLAGS) -c ./merge_sort/Merge_sort.c

sort.o:sort.c sort.h
	$(CC) $(CFLAGS) -c sort.c

clean:
	rm -f *.o 
	(cd heap_sort; make clean)
	(cd merge_sort; make clean)
	(cd insertion_sort; make clean)
