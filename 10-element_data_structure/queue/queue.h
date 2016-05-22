#ifndef __QUEUE_H
#define __QUEUE_H

typedef Queue *Queue_ptr;

void Queue_init(Queue_ptr queue, int capacity);
void Queue_enqueue(Queue_ptr queue, int target);
int Queue_dequeue(Queue_ptr queue);
void Queue_delete(Queue_ptr queue);
void Queue_empty(Queue_ptr queue);

#endif  // __QUEUE_H
