#include <stdio.h>
#include <stdlib.h>
#include "Binary_search.h"


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

