#include <stdio.h>
#include <stdlib.h>
#include "Quick_sort.h"

static void swap(int *source1,int *source2);
static int Quick_sort_partition(int *source,int left_index,int right_index);
static int Quick_sort_partition_hoare(int *source,int left_index,int right_index);
void Quick_sort(int *source,int left_index,int right_index);
void Quick_sort_hoare(int *source,int left_index,int right_index);

void Quick_sort(int *source,int left_index,int right_index)
{
	int mid_index;
	if(left_index<right_index)
	{
		mid_index = Quick_sort_partition(source,left_index,right_index);
		Quick_sort(source,left_index,mid_index-1);
		Quick_sort(source,mid_index+1,right_index);
	}	
}

static int Quick_sort_partition(int *source,int left_index,int right_index)
{
	int i,j;
	int key = source[right_index];

	i = left_index-1;

	for(j=left_index;j<right_index;j++)
	{
		if(source[j]<key)
		{
			i++;
			swap(&source[i],&source[j]);
		}
	}
	swap(&source[i+1],&source[right_index]);

	return i+1;
}

static void swap(int *source1,int *source2)
{
	int tmp = *source1;
	*source1 = *source2;
	*source2 = tmp;
}

void Quick_sort_hoare(int *source,int left_index,int right_index)
{
	int mid_index;
	if(left_index<right_index)
	{
		mid_index = Quick_sort_partition_hoare(source,left_index,right_index);
		Quick_sort(source,left_index,mid_index-1);
		Quick_sort(source,mid_index+1,right_index);
	}	
}

static int Quick_sort_partition_hoare(int *source,int left_index,int right_index)
{
	int i,j;
	int key = source[right_index];
	
	i = left_index-1;
	j = right_index+1;
	
	while(1)
	{
		while(source[j]>key)
		{ j--; }
		while(source[i]<key)
		{
			i++;
		}

		if(i<j)
			swap(&source[i],&source[j]);
		else
			return j;
	}	
}

void Quick_sort_call(int *source,int size)
{
	Quick_sort(source,0,size-1);	
}

void Quick_sort_hoare_call(int *source,int size)
{
	Quick_sort_hoare(source,0,size-1);
}
