#ifndef __STACK_H
#define __STACK_H

typedef struct stack {
  int *stack_base;
  int *stack_top;
  int stack_size;
} Stack;

typedef Stack(*Stack_ptr);

void Stack_Init(Stack_ptr stack, int size);
void Stack_push(Stack_ptr stack, int target);
int Stack_pop(Stack_ptr stack);
int Stack_is_empty(Stack_ptr stack);
void Stack_delete(Stack_ptr stack);

#endif  // __STACK_H
