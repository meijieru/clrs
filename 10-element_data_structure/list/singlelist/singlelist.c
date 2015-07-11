#include "singlelist.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

int main()
{
	element_type i;
	Node *root = ListInit();
	for(i=0;i<MAX_SIZE;i++)
		ListInsert(&root,i);
	ListCheck(&root);
	ListReserve(&root);
	ListCheck(&root);
	ListDelete(&root,9);
	ListDelete(&root,3);
	ListCheck(&root);
	printf("searched:%d\n",ListSearch(&root,4)->data);
	ListEmpty(&root);
}

Node *ListSearch(Node **root,element_type target)
{
	Node *current;
	while((current=*root)!=NULL&&(current->data!=target))
		root = &(*root)->next;
	return *root;
}

Node *ListInit(void)
{
	return (Node *)NULL;
}

Node *ListInsert(Node **root,element_type target)
{
	Node *new = malloc(sizeof(Node));
	new->data = target;
	new->next = *root;
	*root = new;
	return *root;
}

/**
 * @brief avoid to check if the Node to be deleted is the first one
 *
 * @param root
 * @param target
 *
 * @return 
 */
Node *ListDelete(Node **root,element_type target)
{
	Node *current;
	while((current=*root)!=NULL&&(target!=current->data))
	{
		root = &(*root)->next;
	}
	*root = (*root)->next;
	free(current);
	return *root;
}

Node *ListCheck(Node **root)
{
	int size_cout = 1;
	Node *current;
	while((current=*root)!=NULL)
	{
		printf("the %d Node is:%d\n",size_cout,current->data);
		size_cout++;
		root = &(*root)->next;
	}
	return *root;
}

Node *ListEmpty(Node **root)
{
	Node *current,*next=*root;
	while((current=next)!=NULL)
	{
		next = current->next;
		free(current);
	}
	return NULL;
}

Node *ListReserve(Node **root)
{
	Node *current=*root,*next=current->next;
	Node *tmp;
	current->next = NULL;
	while(next!=NULL)
	{
		tmp = next->next;
		next->next = current;
		current=next;
		next = tmp;
	}
	*root = current;
	return *root;
}
