#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

int main()
{
	element_type i;
	Node **root = ListInit();
	for(i=0;i<MAX_SIZE;i++)
		ListInsert(root,i);
	ListCheck(root);
	ListReserve(root);
	ListCheck(root);
	ListDelete(root,3);
	ListCheck(root);
	
}

/* Node *ListSearch(Node **root) */
/* { */
	
/* } */

Node **ListInit(void)
{
	return (Node **)NULL;
}

Node *ListInsert(Node **root)
{
	
}
