#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "select.h"

#define SIZE 20

void swap(int *source1,int *source2);
int  Select(int *source,int size,int power);
int  Random(int left_bound,int right_bound);
int  Random_select(int *source,int left_bound,int right_bound,int power);
int  Random_partition(int *source,int left_bound,int right_bound);

void Insertion(int *source,int left_index,int right_index);

int main()
{
	int source[SIZE];
   	FILE *file1;
	int i;
	int result;

	file1 = fopen("source.txt","r");
	for(i=0;i<SIZE;i++)
		fscanf(file1,"%d",&source[i]);	//notice the type

    for(i=0;i<SIZE;i++)
		printf("%d ",source[i]);	//notice the type

    printf("\n");

	fclose(file1);
	result = Select(source,SIZE,2);
    printf("result:%d\n",result);


    Insertion(source,0,SIZE-1);
    for(i=0;i<SIZE;i++)
		printf("%d ",source[i]);	//notice the type
    printf("\n");


	return 0;
}

int Select(int *source,int size,int power)
{
    return Random_select(source,0,size-1,power);
}

int Random(int left_bound,int right_bound)
{
   return (left_bound+(rand()%(right_bound-left_bound+1)));
}

int Random_select(int *source,int left_index,int right_index,int power)
{
    int mid_index;
    int tmp;

    if(right_index==left_index)
        return source[left_index];

    mid_index = Random_partition(source,left_index,right_index);
    tmp = mid_index-left_index+1;

    if(tmp==power)
        return source[mid_index];

    else if(power<tmp)
        return Random_select(source,left_index,mid_index-1,power);  //will not be call with the length of 0
    else
        return Random_select(source,mid_index+1,right_index,power-tmp);
}

int Random_partition(int *source,int left_index,int right_index)
{
    swap(&source[Random(left_index,right_index)],&source[right_index]);

    int tmp = source[right_index];
    int i = left_index-1;
    int j;

    for(j=left_index;j<right_index;j++)
    {
        if(source[j]<=tmp)
        {
            i++;
            swap(&source[i],&source[j]);
        }
    }

    swap(&source[i+1],&source[right_index]);

    return i+1;
}

void swap(int *source1,int *source2)
{
    int tmp = *source1;
    *source1 = *source2;
    *source2 = tmp;
}


void Insertion(int *source,int left_index,int right_index)
{
	int i,j;
	int k;
	int key;
	int size = right_index-left_index+1;

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


    for(k=left_index;k<right_index+1;k++)
        printf("%d ",source[k]);

    printf(" tags\n");

	}
}


