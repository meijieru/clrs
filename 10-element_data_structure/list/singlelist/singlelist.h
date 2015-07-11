#ifndef __LIST_H
#define __LIST_H

typedef int element_type;
typedef struct node
{
	struct node *next;
	element_type data;
}Node;

// #define NEXT_NODE(ptr) &

Node *ListSearch(Node **root);
Node *ListInsert(Node **root,element_type target);
Node *ListInit(void);
Node *ListDelete(Node **root,element_type target);
Node *ListReserve(Node **root);
Node *ListCheck(Node **root);
Node *ListEmpty(Node **root);

#endif // __LIST_H
