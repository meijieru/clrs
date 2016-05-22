#ifndef __HEAP_SORT_H
#define __HEAP_SORT_H

#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)

typedef struct heap {
  int *array;
  int heap_size;
  int length;
} heap;

void Heap_sort_call(int *source, int size);
void Max_Heapify(heap *source, int index);
void Heap_Init_Insertion(heap *source);
void Heap_Init(heap *source);
void Heap_Init_Test(int *source, int *source2, int size);
void Heap_sort(heap *source);
void swap(int *a, int *b);

/* priority queue */
void priority_queue(int *source, int size);
int Max_Heap_Maximum(heap *source);
void Max_Heap_Insert(heap *source, int key);
int Max_Heap_Extract_Max(heap *source);
void Max_Heap_Increase_Key(heap *source, int index, int key);
void Max_Heap_Debug(heap *source);

#endif  //__HEAP_SORT_H
