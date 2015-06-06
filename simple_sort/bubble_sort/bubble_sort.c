#include <stdio.h>
#include "bubble_sort.h"

static void swap(int *source1,int *source2);


void Bubble_sort(int *source,int left_index,int right_index)
{
    int i,j;
    int size = right_index-left_index+1;

    for(i=0;i<size-1;i++)
    {
        for(j=size-1;j>i-1;j--)
        {
            if(source[j]>source[j+1])
                swap(&source[j],&source[j+1]);
        }
    }
}

static void swap(int *source1,int *source2)
{
    int tmp;
    tmp = *source2;
    *source2 = *source1;
    *source1 = tmp;
}

