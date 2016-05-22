#include "xorlist.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10
#define DISPATCH_PREV(x, y) ()
#define DISPATCH_NEXT(x, y)

int main() {
  element_type i;
  Xorlist *root = XorListInit();
  Node *result;
  for (i = 0; i < MAX_SIZE; i++) XorListInsert(root, i + 1);
  XorListCheck(root);
  printf("tag1\n");
  XorListReserve(root);
  XorListCheck(root);
  printf("tag2\n");
  XorListDelete(root, 10);
  XorListDelete(root, 1);
  XorListDelete(root, 3);
  XorListCheck(root);
  result = XorListSearch(root, 8);
  printf("searched:%d\n", result ? result->data : -1);
  XorListEmpty(root);
  root = NULL;
}

inline Node *Xor(Node *x, Node *y) { return (Node *)((int)x ^ (int)y); }

Node *XorListSearch(Xorlist *root, element_type target) {
  Node *current = root->list_head;
  Node *prev = NULL;
  Node *next;

  while (current != NULL && current->data != target) {
    next = Xor(current->next, prev);
    prev = current;
    current = next;
  }
  return current;
}

Xorlist *XorListInit(void) {
  Xorlist *new = malloc(sizeof(Xorlist));
  new->list_head = NULL;
  new->list_tail = NULL;
  return new;
}

Node *XorListInsert(Xorlist *root, element_type target) {
  Node *new = malloc(sizeof(Xorlist));
  new->data = target;
  new->next = Xor(NULL, root->list_tail);
  if (root->list_tail)
    root->list_tail->next = Xor(new, Xor(NULL, root->list_tail->next));
  if (!root->list_head) root->list_head = new;
  root->list_tail = new;
  return new;
}

Node *XorListDelete(Xorlist *root, element_type target) {
  Node *current = root->list_head;
  Node *prev = NULL;
  Node *next;
  while (current != NULL) {
    if (current->data == target) {
      next = Xor(prev, current->next);

      if (prev == NULL)
        root->list_head = next;
      else
        prev->next = Xor(next, Xor(prev->next, current));

      if (next == NULL)
        root->list_tail = prev;
      else
        next->next = Xor(prev, Xor(next->next, current));
      free(current);
      return prev;
    }
    next = Xor(prev, current->next);
    prev = current;
    current = next;
  }
  return root->list_head;
}

Node *XorListCheck(Xorlist *root) {
  int size_cout = 1;
  Node *prev = NULL;
  Node *current = root->list_head;
  Node *next;
  while (current != NULL) {
    printf("the %d node is:%d\n", size_cout++, current->data);
    next = Xor(current->next, prev);
    prev = current;
    current = next;
  }
  return root->list_tail;
}

Xorlist *XorListEmpty(Xorlist *root) {
  Node *current = root->list_head;
  Node *prev = NULL;
  Node *next;
  while (current) {
    next = Xor(current->next, prev);
    free(current);
    prev = current;
    current = next;
  }
  free(root);
  return NULL;
}

Node *XorListReserve(Xorlist *root) {
  Node *tmp = root->list_tail;
  root->list_tail = root->list_head;
  root->list_head = tmp;
  return root->list_head;
}
