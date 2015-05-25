#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Insertion_sort.h"

#define SIZE 20 


/**
 * @brief 
 *
 * @param source
 * @param size
 */
void Insertion_sort_call(int *source,int size)
{
	int i;

	Insertion(source,size);	
	/* Insertion_Recursive(source,size);	 */

	for(i=0;i<size;i++)
		printf("%d\n",source[i]);
}

/**
 * @brief can be improved with binary-search to nlgn 
 *
 * @param source
 * @param size
 */
void Insertion(int *source,int size)
{
	int i,j;
	int key;
	for(i=1;i<=size;i++)
	{
		j = i-1;
		key = source[i];
		while((key<source[j])&&(j>=0))
		{
			source[j+1] = source[j];
			j--;
		}
		source[j+1] = key;
	}
}

/**
 * @brief 
 *
 * @param source
 * @param size
 */
void Insertion_Recursive(int *source,int size)
{
	int i,j;
	int key;
	if(size>1)
		Insertion_Recursive(source,size-1);

	for(i=1;i<=size;i++)
	{
		j = i-1;
		key = source[i];
		while((key<source[j])&&(j>=0))
		{
			source[j+1] = source[j];
			j--;
		}
		source[j+1] = key;
	}
}

