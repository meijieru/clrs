#ifndef __DOUBLE_LIST_H
#define __DOUBLE_LIST_H

typedef int element_type;
typedef struct node
{
	struct node *next;
	struct node *prev;
	element_type data;
}Node;


Node *DoubleListSearch(Node *root,element_type target);
Node *DoubleListInsert(Node *root,element_type target);
Node *DoubleListInit(void);
Node *DoubleListDelete(Node *root,element_type target);
Node *DoubleListReserve(Node *root);
Node *DoubleListCheck(Node *root);
Node *DoubleListEmpty(Node *root);

#endif // __DOUBLE_LIST_H
