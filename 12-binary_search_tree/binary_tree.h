#ifndef __BINARY_TREE_H
#define __BINARY_TREE_H

typedef int element_type;
typedef struct node
{
	struct node *father;
	struct node *left;
	struct node *right;
	element_type data;
}Node;

Node *BinaryTreeInit(void);
Node *BinaryTreeSearch(Node *root,element_type target);
Node *BinaryTreeEmpty(Node *root);
Node *BinaryTreeDelete(Node *root,element_type target);
Node *BinaryTreeReserve(Node *root);

#endif // __BINARY_TREE_H
