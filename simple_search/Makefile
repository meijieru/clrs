
all:search.o Binary_search.o Random_search.o
	gcc -O1 -g -o target search.o Binary_search.o Random_search.o

Random_search.o:./Random_search/Random_search.c ./Random_search/Random_search.h
	gcc -O1 -g -c ./Random_search/Random_search.c

search.o:search.c search.h
	gcc -O1 -g -c search.c

Binary_search.o:binary_search/Binary_search.c binary_search/Binary_search.h
	gcc -O1 -g -c binary_search/Binary_search.c

clean:
	rm -f *.o
	rm target
