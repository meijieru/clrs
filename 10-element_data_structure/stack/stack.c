#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define STACK_SIZE 10
#define TRUE 1
#define FALSE 0

int main() {
  Stack stack;
  int i;

  Stack_Init(&stack, STACK_SIZE);
  for (i = 0; i < STACK_SIZE; i++) {
    Stack_push(&stack, i);
  }
  printf("\n");
  for (i = 0; i < STACK_SIZE; i++) {
    printf("%d ", Stack_pop(&stack));
  }
  printf("\n");
  Stack_delete(&stack);

  return 0;
}

void Stack_Init(Stack_ptr stack, int size) {
  stack->stack_base = (int *)malloc(sizeof(int) * STACK_SIZE);
  stack->stack_top = stack->stack_base - 1;
  if (stack->stack_top == NULL) exit(0);
  stack->stack_size = size;
}

void Stack_push(Stack_ptr stack, int target) {
  if (stack->stack_top >= stack->stack_base + stack->stack_size - 1) exit(1);

  stack->stack_top++;
  *stack->stack_top = target;
}

int Stack_pop(Stack_ptr stack) {
  int tmp;
  if (stack->stack_top == stack->stack_base - 1)
    exit(2);
  else {
    return *stack->stack_top--;
  }
}

int Stack_is_empty(Stack_ptr stack) {
  if (stack->stack_top == stack->stack_base - 1)
    return TRUE;
  else
    return FALSE;
}

void Stack_delete(Stack_ptr stack) {
  stack->stack_size = 0;
  free(stack->stack_base);
  stack->stack_base = NULL;
  stack->stack_top = NULL;
}
