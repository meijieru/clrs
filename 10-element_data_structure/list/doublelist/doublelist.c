#include "doublelist.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

int main()
{
	element_type i;
	Node *root = DoubleListInit();
	Node *result;
	for(i=0;i<MAX_SIZE;i++)
		DoubleListInsert(root,i+1);
	DoubleListCheck(root);
	printf("tag1\n");
	/* printf("DoubleListReserve:%d\n",DoubleListReserve(root)->next->next->data);	 */
	DoubleListCheck(root);
	printf("tag2\n");
	DoubleListDelete(root,9);
	DoubleListDelete(root,3);
	DoubleListCheck(root);
	result = DoubleListSearch(root,100);
	printf("searched:%d\n",result ? result->data:-1);
	DoubleListEmpty(root);
	root = NULL;
}

Node *DoubleListSearch(Node *root,element_type target)
{
	element_type tmp = root->data;
	Node *current = root;
	root->data = target;
	while((current=current->next)&&(current->data!=target));
	root->data = tmp;
	if(current==root)
		return NULL;
	return current;
}

Node *DoubleListInit(void)
{
	Node *new = malloc(sizeof(Node));
	new->data = 0;
	new->next = new;
	new->prev =new;
	return new;
}

Node *DoubleListInsert(Node *root,element_type target)
{
	Node *new = malloc(sizeof(Node));
	new->data = target;
	new->prev = root;
	new->next = root->next;
	root->next->prev = new;
	root->next = new;
	return root;
}

Node *DoubleListDelete(Node *root,element_type target)
{
	Node *current = DoubleListSearch(root,target);
	if(current==NULL)
		return root;
	current->prev->next = current->next;
	current->next->prev = current->prev;
	free(current);
	return root;
}

Node *DoubleListCheck(Node *root)
{
	int size_cout = 1;
	Node *current = root;
	while((current=current->next)!=root)
		printf("the %d node is:%d\n",size_cout++,current->data);
	return root;
}

/**
 * @brief notice to reset the root
 *
 * @param root
 *
 * @return 
 */
Node *DoubleListEmpty(Node *root)
{
	Node *current,*next=root->next;
	while((current=next)!=root)
	{
		next = current->next;
		free(current);
	}
	free(root);
	return NULL;
}

Node *DoubleListReserve(Node *root)
{
	Node *tmp,*current=root,*next=current->next;
	while((current=next)!=root)
	{
		next = current->next;
		tmp = current->next;
		current->next = current->prev;
		current->prev = tmp;
	}
	tmp = current->next;
	current->next = current->prev;
	current->prev = tmp;
	
	return current;
}
