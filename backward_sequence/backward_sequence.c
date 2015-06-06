#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define SIZE 20
#define LAST_ELEMENT INT_MAX

void Backward_Sequence_Count(int *source,int size);

void Backward_Sequence_Count_Merge(int *source,int left_index,int mid_index,int right_index);
void Backward_Sequence_Count_Merge_Count(int *source,int left_index,int right_index);

int count;

int main()
{
	int source[SIZE];
   	FILE *file1;
	int i;

	file1 = fopen("source.txt","r");
	for(i=0;i<SIZE;i++)
		fscanf(file1,"%d",&source[i]);	//notice the type

	fclose(file1);
	Backward_Sequence_Count(source,SIZE);
	return 0;
}

void Backward_Sequence_Count(int *source,int size)
{
    Backward_Sequence_Count_Merge_Count(source,0,size-1);
    printf("%d\n",count);
}

void Backward_Sequence_Count_Merge_Count(int *source,int left_index,int right_index)
{
    int mid_index;
	if(left_index<right_index)
	{
		mid_index = (left_index+right_index)/2;
		Backward_Sequence_Count_Merge_Count(source,left_index,mid_index);
		Backward_Sequence_Count_Merge_Count(source,mid_index+1,right_index);
		Backward_Sequence_Count_Merge(source,left_index,mid_index,right_index);
	}
}

void Backward_Sequence_Count_Merge(int *source,int left_index,int mid_index,int right_index)
{

	int count1,count2,count;
	int i,j,k;
	int *array1,*array2;

	count1 = mid_index-left_index+1;
	count2 = right_index-mid_index;
	count = count1+count2;

	array1 = malloc(sizeof(int)*(count1+1));
	if(array1 == NULL)
		exit(0);
   	array2 = malloc(sizeof(int)*(count2+1));
	if(array2 == NULL)
		exit(0);

	for(i=0;i<count1;i++)
		array1[i] = source[left_index+i];
	array1[count1] = LAST_ELEMENT;

	for(i=0;i<count2;i++)
		array2[i] = source[mid_index+i+1];
	array2[count2] = LAST_ELEMENT;

	j=0;
	k=0;
	for(i=0;i<count;i++)
	{
		if(array1[j]<array2[k])
		{
			source[left_index+i] = array1[j];
			j++;
		}
		else
		{
			source[left_index+i] = array2[k];
			k++;
			count+=count1-j;
		}
	}

	free(array1);
	free(array2);
}
