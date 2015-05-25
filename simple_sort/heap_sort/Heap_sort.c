#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Heap_sort.h"



/**
 * @brief 
 *
 * @param source
 * @param size
 */
void Heap_sort_call(int *source,int size)
{
	int i;
	heap source_heap;

	source_heap.array = source;
	source_heap.heap_size = size;
	source_heap.length = size;

	Heap_sort(&source_heap);

	for(i=0;i<size;i++)
	{
		printf("%d ",source[i]);
	}
	printf("\n");
	
}

/**
 * @brief 
 *
 * @param source
 * @param size
 */
void priority_queue(int *source,int size)
{
	heap source_heap;
	int max;

	source_heap.array = source;
	source_heap.heap_size = size;
	source_heap.length = size;

	Heap_Init(&source_heap);
	Max_Heap_Debug(&source_heap);

	max = Max_Heap_Maximum(&source_heap);
	printf("%d\n",max);
	max = Max_Heap_Extract_Max(&source_heap);
	printf("%d\n",max);
	max = Max_Heap_Extract_Max(&source_heap);
	printf("%d\n",max);
	Max_Heap_Debug(&source_heap);
	printf("tags1\n");
	Max_Heap_Insert(&source_heap,3);
	Max_Heap_Debug(&source_heap);
	printf("tags2\n");
	Max_Heap_Insert(&source_heap,300);
	Max_Heap_Debug(&source_heap);
	printf("tags3\n");
	/* Max_Heap_Increase_Key(&source_heap,10,920); */
	/* Max_Heap_Debug(&source_heap); */
	/* printf("tags4\n"); */
}


/**
 * @brief O(logn)
 *
 * @param source
 * @param index
 */
void Max_Heapify(heap *source,int index)
{
	int left_index;
	int right_index;
	int largest = 0;
	int size = source->heap_size;
	

	while(1)
	{
		left_index = LEFT(index);
		right_index = RIGHT(index);

		if((left_index<size)&&(source->array[left_index]>source->array[index]))
			largest = left_index;
		else
			largest = index;

		if((right_index<size)&&(source->array[right_index]>source->array[largest]))
				largest = right_index;
		
		if(largest!=index)
		{
			swap(&(source->array[index]),&(source->array[largest]));
			index = largest;
		}
		else
		{
			return;
		}
	}
}

/**
 * @brief 
 *
 * @param source
 * @param source2
 * @param size
 */
void Heap_Init_Test(int *source,int *source2,int size)
{
	int i;
	heap source_heap,source_heap2;

	source_heap.array = source;
	source_heap.heap_size = size;
	source_heap.length = size;

	source_heap2.array = source;
	source_heap2.heap_size = size;
	source_heap2.length = size;


	Heap_Init(&source_heap);
	for(i=0;i<size;i++)
	{
		printf("%d ",source[i]);
	}
	printf("\n");
	printf("source_heap.heap_size:%d,source_heap.length:%d\n",source_heap.heap_size,source_heap.length);
	
	Heap_Init_Insertion(&source_heap2);
	for(i=0;i<size;i++)
	{
		printf("%d ",source2[i]);
	}
	printf("\n");
	printf("source_heap.heap_size:%d,source_heap.length:%d\n",source_heap2.heap_size,source_heap2.length);
	
}
/**
 * @brief 
 *
 * @param source
 */
void Heap_Init(heap *source)
{
	int i;
	source->heap_size = source->length;
	for(i=(source->length)/2-1;i>=0;i--)
	{
		Max_Heapify(source,i);
	}

}

void Heap_Init_Insertion(heap *source)
{
	int i;
	source->heap_size = 1;
	for(i=1;i<source->length;i++)
	{
		Max_Heap_Insert(source,source->array[i]);	
	}
}

/**
 * @brief O(nlgn) omiga(nlgn)
 *
 * @param source
 */
void Heap_sort(heap *source)
{
	int i;
	int j;

	/* Heap_Init(source); */
	Heap_Init_Insertion(source);
	Max_Heap_Debug(source);	
	printf("tags1\n");

	for(i=(source->heap_size-1);i>=1;i--)
	{
		swap(&(source->array[0]),&(source->array[i]));
		source->heap_size = source->heap_size-1;
		Max_Heapify(source,0);
		for(j=0;j<source->length;j++)
		{
			printf("%d ",source->array[j]);
		}
		printf("\n");
	}	
}

/**
 * @brief 
 *
 * @param a
 * @param b
 */
void swap(int *a,int *b)
{
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
 * @brief 
 *
 * @param source
 *
 * @return 
 */
int Max_Heap_Maximum(heap *source)
{
	return source->array[0];	
}

/**
 * @brief 
 *
 * @param source
 *
 * @return 
 */
int Max_Heap_Extract_Max(heap *source)
{
	int max;

	if(source->heap_size<1)
		printf("heap underflow\n");
	
	max = source->array[0];
	source->array[0] = source->array[source->heap_size-1];
	source->heap_size -= 1;
	Max_Heapify(source,0);

	return max;
}

/**
 * @brief 
 *
 * @param source
 * @param key
 */
void Max_Heap_Insert(heap *source,int key)
{
	if(source->heap_size>=source->length)
		printf("heap overflow");
	
	source->array[source->heap_size] = INT_MIN;
	Max_Heap_Increase_Key(source,source->heap_size,key);

	source->heap_size += 1;	
}

/**
 * @brief 
 *
 * @param source
 * @param index
 * @param key
 */
void Max_Heap_Increase_Key(heap *source,int index,int key)
{
	if(key<source->array[index])
		printf("new key is smaller");

	source->array[index] = key;
	while((index>0)&&((source->array[PARENT(index)])<key))
	{
		/* printf("index:%d\n",index); */
		source->array[index] = source->array[PARENT(index)];
		index = PARENT(index);
		/* printf("index:%d\n",index); */
		
		/* swap(&(source->array[index]),&source->array[PARENT(index)]); */
		/* index = PARENT(index); */
	}
	source->array[index] = key;
}

/**
 * @brief 
 *
 * @param source
 */
void Max_Heap_Debug(heap *source)
{
	int i,size=source->heap_size;

	for(i=0;i<size;i++)
	{
		printf("%d ",source->array[i]);
	}
	printf("\n");
}
