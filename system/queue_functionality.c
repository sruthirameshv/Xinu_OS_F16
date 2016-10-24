#include<future.h>

q* init_q()
{
       
        q *Q;
        Q = (q *)getmem(sizeof(q));
        Q->vals = (pid32 *)getmem(sizeof(pid32)*200);
        Q->size = 0;
        Q->capacity = 200;
        Q->front = 0;
        Q->rear = 0;
        return Q;
}



void en_queue(q *Q,pid32 val)
{
       
        if(Q->size == Q->capacity)
        {
               return;
        }
        else
        {  Q->size++;

                Q->rear = Q->rear + 1;
                
                if(Q->rear == Q->capacity)
                {
                        Q->rear = 0;
                }
                Q->vals[Q->rear] = val;
        }
        return;
} 
pid32 d_queue(q *Q)
{	
        
        if(Q->size==0)
        {
                return -1;
        }
        else
        {		
                Q->size--;
	        Q->front++;
                if(Q->front ==  Q->capacity)
                {
                        Q->front=0;
                }
		
        }
        return 1;
}
int isempty_q(q *Q)
{	
        
        if(Q->size==0)
        {             
                return 1;
        }
        
        return 0;
}
pid32 get_q(q *Q)
{
        return Q->vals[Q->front];
}
