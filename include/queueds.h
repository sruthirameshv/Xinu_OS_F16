#ifndef _QUEUEDS_H_
#define _QUEUEDS_H_

#define QUEUEDS_LENGTH 100

typedef struct queue_struct {
	int *queue_data;
	int front;
	int rear;
	int count;
} queue;

int queue_add(queue *queue, int item);
int queue_remove(queue *queue);

#endif
