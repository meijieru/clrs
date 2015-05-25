#include <stdio.h>
#include <stdlib.h>
#include "Random_search.h"

/**
 * @brief 
 *
 * @param low_bound
 * @param high_bound
 *
 * @return 
 */
static int Rand(int low_bound,int high_bound)
{
	int k;
	k=low_bound+rand()%(high_bound-low_bound+1);

	return k;
}

/**
 * @brief 
 *
 * @param source1
 * @param source2
 */
static void swap(int *source1,int *source2)
{
	int tmp = *source1;
	*source1 = *source2;
	*source2 = tmp;
}

/**
 * @brief 
 *
 * @param source
 * @param left_index
 * @param right_index
 */
static void Random_in_place(int *source,int left_index,int right_index)
{
	int i;

	for(i=left_index;i<right_index+1;i++)
	{
		swap(&(source[i]),&(source[Rand(i,right_index)]));
	}

}

/**
 * @brief 
 *
 * @param source
 * @param left_index
 * @param right_index
 * @param target
 *
 * @return 
 */
int Random_search(int *source,int left_index,int right_index,int target)
{
	int index = Rand(left_index,right_index);

	while(source[index]!=target)
	{
		index = Rand(left_index,right_index);
	}

	return index;
}

/**
 * @brief 
 *
 * @param source
 * @param left_index
 * @param right_index
 * @param target
 *
 * @return 
 */
int Deterministic_search(int *source,int left_index,int right_index,int target)
{
	int i=left_index;

	while((source[i]!=target)&&(i<right_index+1))
	{
		i++;
	}

	return i;
}

/**
 * @brief 
 *
 * @param source
 * @param left_index
 * @param right_index
 * @param target
 *
 * @return 
 */
int Scramble_search(int *source,int left_index,int right_index,int target)
{
	int i;

	Random_in_place(source,left_index,right_index);	

	i = Deterministic_search(source,left_index,right_index,target);

	return i;
}

