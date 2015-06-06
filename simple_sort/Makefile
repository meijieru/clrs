CC = gcc
CFLAGS = -O1 -g

all:sort.o Heap_sort.o Merge_sort.o Insertion_sort.o Quick_sort.o bubble_sort.o select_sort.o
	$(CC) $(CFLAGS) -o target sort.o Heap_sort.o Merge_sort.o Insertion_sort.o Quick_sort.o bubble_sort.o select_sort.o

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

bubble_sort.o:./bubble_sort/bubble_sort.c ./bubble_sort/bubble_sort.h
	$(CC) $(CFLAGS) -c ./bubble_sort/bubble_sort.c

select_sort.o:./select_sort/select_sort.c ./select_sort/select_sort.h
	$(CC) $(CFLAGS) -c ./select_sort/select_sort.c

clean:
	rm -f *.o 
	rm target
	(cd heap_sort; rm -f *.o) 
	(cd merge_sort; rm -f *.o) 
	(cd insertion_sort; rm -f *.o)
	(cd select_sort; rm -f *.o) 
	(cd bubble_sort; rm -f *.o) 
	
