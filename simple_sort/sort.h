#ifndef __SORT_H
#define __SORT_H

#include "heap_sort/Heap_sort.h"
#include "merge_sort/Merge_sort.h"
#include "insertion_sort/Insertion_sort.h"
#include "quick_sort/Quick_sort.h"

typedef void (*function)(int *,int);

void sort(int *source,int size);

#define SIZE 20
#define FUNCTION_SIZE 5

#endif //__SORT_H
