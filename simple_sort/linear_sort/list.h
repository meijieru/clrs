#ifndef __LIST_H
#define __LIST_H

typedef struct node
{
	struct node * next_node;
	int body;
}node;

typedef node (*node_ptr);

#endif // __LIST_H
