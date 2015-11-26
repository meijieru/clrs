#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "sort.h"


int main()
{
	int source[SIZE];
	/* int source2[SIZE]; */
   	FILE *file1;
	int i;

	file1 = fopen("./random.txt","r");
	for(i=0;i<SIZE;i++)
		fscanf(file1,"%d",&source[i]);	//notice the type
	fclose(file1);

	/* for(i=0;i<SIZE;i++) */
		/* source2[i] = source[i]; */


	/* call both of them may conflict with the Heap_Init_Insertion,the heap_size may be wrong */
	/* Quick_sort_hoare_call(source,SIZE); */
	/* for(i=0;i<SIZE;i++) */
		/* printf("%d ",source[i]); */
	sort(source,SIZE);
	/* Quick_sort_call(source,SIZE); */
	/* priority_queue(source,SIZE); */

	/* Heap_Init_Test(source,source2,SIZE);	 */
	return 0;
}

/**
 * @brief
 *
 * @param source
 * @param size
 */
void sort(int *source,int size)
{
	int i,k;
	int *ptr_tmp;
	clock_t start,end;
	function function_source[FUNCTION_SIZE];

	function_source[0] = &Heap_sort_call;
	function_source[1] = &Merge_sort_call;
	function_source[2] = &Insertion_sort_call;
	function_source[3] = &Select_sort_call;
	function_source[4] = &Quick_sort_call;
	function_source[5] = &Bubble_sort_call;

	for(i=0;i<FUNCTION_SIZE;i++)
	{
		ptr_tmp = (int *)malloc(sizeof(int)*size);
		if(ptr_tmp==NULL)
			exit(0);
		else
		{
			for(k=0;k<size;k++)
				ptr_tmp[k] = source[k];
		}

		/* for(k=0;k<size;k++) */
			/* printf("%d ",ptr_tmp[k]); */

		/* printf("tags1\n"); */

		start = clock();
		/* Merge_sort_call(ptr_tmp,size); */
		function_source[i](ptr_tmp,size);
		end = clock();
		printf("time wasted:%ld\n",(end-start));
		free(ptr_tmp);
	}
}

