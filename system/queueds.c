#include<xinu.h>

int queue_init(queue *queue){
    queue->queue_data = (int *)getmem(QUEUEDS_LENGTH);
    queue->count = 0;
    queue->front = 0;
    queue->rear = -1;
}

int queue_add(queue *queue, int item){
    if(queue->rear + 1 > QUEUEDS_LENGTH){
        return SYSERR;
    }
    queue->queue_data[++queue->rear] = item;
    queue->count++;
    return OK;
}

int queue_remove(queue *queue){
    if(queue->count < 1){
        return SYSERR;
    }
    queue->count--;
    return queue->queue_data[queue->front++];
}
