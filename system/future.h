#include<prodcons.h>

#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         
#define FUTURE_EXCLUSIVE  1	
#define FUTURE_QUEUE	  2      
#define FUTURE_SHARED	  3        

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
   pid32 tid;
   q *set_q;     
   q *get_q;
} future;

/* Interface for system call */
uint32 future_prod(future *fut);
uint32 future_cons(future *fut);
typedef struct futent future;
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);
/*Queue Functionality*/
q* init_q();
int d_queue(q *Q);
int isempty_q(q *Q);
pid32 get_q(q *Q);
void en_queue(q *Q,int); 

