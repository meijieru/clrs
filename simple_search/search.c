#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "search.h"

int search(int *source,int size,int target);
void Insertion(int *source,int size);

int main()
{
	int source[SIZE];
   	FILE *file1;
	int i;
	int result;

	file1 = fopen("./source.txt","r");
	for(i=0;i<SIZE;i++)
		fscanf(file1,"%d",&source[i]);	//notice the type
	fclose(file1);

	Insertion(source,SIZE);

	for(i=0;i<SIZE;i++)
	{
		printf("%d ",source[i]);
	}
	printf("\n");

	result = search(source,SIZE,342);
	printf("the result is:%d\n",result);

   	return 0;
}

/**
 * @brief 
 *
 * @param source
 * @param size
 * @param target
 *
 * @return 
 */
int search(int *source,int size,int target)
{
	return Binary_search(source,0,size-1,target,ASCENDING_ORDER);
	/* return Random_search(source,0,size-1,target);  */
	/* return Deterministic_search(source,0,size-1,target); */
	/* return Scramble_search(source,0,size-1,target); */
}

/**
 * @brief 
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


