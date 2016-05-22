#ifndef __XOR_LIST_H
#define __XOR_LIST_H

typedef int element_type;

typedef struct node {
  struct node *next;
  element_type data;
} Node;

typedef struct xorlist {
  Node *list_head;
  Node *list_tail;
} Xorlist;

Node *XorListSearch(Xorlist *root, element_type target);
Node *XorListInsert(Xorlist *root, element_type target);
Xorlist *XorListInit(void);
Node *XorListDelete(Xorlist *root, element_type target);
Node *XorListReserve(Xorlist *root);
Node *XorListCheck(Xorlist *root);
Xorlist *XorListEmpty(Xorlist *root);

#endif  // __XOR_LIST_H
