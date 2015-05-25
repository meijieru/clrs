#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 20
#define LAST_ELEMENT INT_MAX 
#define ASCENDING_ORDER 1
#define DESCENDING_ORDER 0


int Binary_search(int *source,int low_index,int high_index,int target,int option);
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
	result = search(source,SIZE,2);
	printf("%d\n",result);
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
	Insertion(source,SIZE);
	return Binary_search(source,0,size-1,target,ASCENDING_ORDER);
}

/**
 * @brief 
 *
 * @param source
 * @param low_index
 * @param high_index
 * @param target
 * @param option
 *
 * @return 
 */
int Binary_search(int *source,int low_index,int high_index,int target,int option)
{
	int mid_index;
	mid_index = (low_index+high_index)/2;

	if(low_index>high_index)
		return -1;

	if(source[mid_index]==target)
		return mid_index;
	else if(source[mid_index]<target)
	{
		if(option == ASCENDING_ORDER)
			return Binary_search(source,mid_index+1,high_index,target,option);
		else
			return Binary_search(source,low_index,mid_index-1,target,option);
	}
	else
	{
		if(option == ASCENDING_ORDER)
			return Binary_search(source,low_index,mid_index-1,target,option);
		else
			return Binary_search(source,mid_index+1,high_index,target,option);
	}
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


