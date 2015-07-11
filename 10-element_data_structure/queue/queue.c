#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define QUEUE_SIZE 10
#define TRUE 1
#define FALSE 0


typedef struct queue
{
    int *queue_head;
    int *queue_tail;
    int *base;
    int size;
    int capacity;
}Queue;

int main()
{
    int i;
    Queue queue;

    Queue_init(&queue,QUEUE_SIZE);
    for(i=0;i<QUEUE_SIZE;i++)
        Queue_enqueue(&queue,i);
    for(i=0;i<QUEUE_SIZE;i++)
        printf("%d ",Queue_dequeue(&queue));
    printf("\n");
    Queue_delete(&queue);

    return 0;
}

void Queue_init(Queue_ptr queue,int capacity)
{
    queue->base = (int *)malloc(sizeof(int)*capacity);
    if(queue->base==NULL)
        exit(0);
    queue->capacity = capacity;
    queue->size = 0;
    queue->queue_head = queue->base;
    queue->queue_tail = queue->base;
}

void Queue_enqueue(Queue_ptr queue,int target)
{
    if(queue->size >= queue->capacity)
        exit(2);

    *queue->queue_tail = target;
    queue->size++;
    if(queue->queue_tail==queue->base+queue->capacity-1)
        queue->queue_tail = queue->base;
    else
        queue->queue_tail++;
}

int Queue_dequeue(Queue_ptr queue)
{
    int tmp;
    if(queue->size==0)
        exit(1);

    tmp = *queue->queue_head;
    queue->size--;
    if(queue->queue_head==queue->base+QUEUE_SIZE-1)
        queue->queue_head = queue->base;
    else
        queue->queue_head++;

    return tmp;
}

void Queue_delete(Queue_ptr queue)
{
    free(queue->base);
    queue->base = NULL;
    queue->queue_head = NULL;
    queue->queue_tail = NULL;
    queue->capacity = 0;
}

void Queue_empty(Queue_ptr queue)
{
    queue->queue_head = queue->base;
    queue->queue_tail = queue->base;
    queue->size = 0;
}
