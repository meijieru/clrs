#include <stdio.h>
#include <stdlib.h>
#include "Linear_sort.h"
#include "list.h"

void Counting_sort(int *source,int *destination,int size,int k);
void Radix_sort(int *source,int length);
void Bucket_sort(int *source,int length);

void Counting_sort(int *source,int *destination,int size,int k)
{
	int i;
	int *count = malloc(sizeof(int)*k);

	if(count==NULL)
	{
		exit(0);
	}

	for(i=0;i<k;i++)
	{
		count[i] = 0;
	}

	for(i=0;i<size;i++)
	{
		count[source[i]]++;	
	}

	for(i=1;i<k;i++)
	{
		count[i]+=count[i-1];
	}

	for(i=size-1;i>=0;i++)
	{
		destination[count[source[i]]] = source[i];
		count[source[i]]--;
	}

	free(count);
}

void Radix_sort(int *source,int length)
{
	int i;
	
	for(i=0;i<length;i++)
	{
		
	}	
}


/**
 * @brief expect the input are equidistribution
 *
 * @param source
 * @param length
 */
void Bucket_sort(int *source,int length)
{
	
}
