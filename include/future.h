#include <prodcons.h>
typedef unsigned int      uint;
#ifndef _FUTURE_H_
#define _FUTURE_H_

/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	
#define FUTURE_SHARED     2
#define FUTURE_QUEUE      3

/*struct  QueueFuture
{
  int size;
  pid32 *p;
  int capacity;
  int left;
  int right;
};

int initializeQueue(struct QueueFuture*, int);
void deallocateQueue(struct QueueFuture*);

//returns 1 on success, and -1 on failure..
int enqueueFuture(struct QueueFuture*, pid32);
pid32 dequeueFuture(struct QueueFuture*);
*/
typedef struct q
	{
	        int capacity;
	        int size;
	        int front;
	        int rear;
	        pid32 *vals;
		//sid32 sem;
	}q;




typedef struct futent
{
   int *value;		
   int flag;		
   int state;         	
   pid32 pid;
   q *set_q;
   q *get_q;
} future;


uint future_prod(future *);
uint future_cons(future *);

typedef struct futent future;
/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);

q* init_q();
	int d_queue(q *Q);
	int isempty_q(q *Q);
	pid32 get_q(q *Q);
	void en_queue(q *Q,int); 
 
#endif /* _FUTURE_H_ */	
