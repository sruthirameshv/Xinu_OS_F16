//#include <xinu.h>
#include<future.h>

syscall future_get(future *f, int* value){
	intmask mask; 
	pid32 p;
	int i;
	mask = disable();
	if(f->flag==FUTURE_SHARED){
		if(f->state==FUTURE_EMPTY||f->state==FUTURE_WAITING){
			f->state=FUTURE_WAITING;			
			en_queue(f->get_q,currpid);
			suspend(currpid);
			
		}
		if(f->state==FUTURE_VALID){
			*value=*(f->value);
			if(isempty_q(f->get_q))
				f->state=FUTURE_EMPTY;
			restore(mask);
			return OK;
			
		}
		if (isempty_q(f->get_q))
	        {
			f->state = FUTURE_EMPTY; 
		} 
		else 
		{
			f->state = FUTURE_WAITING;
		}
		
		

	}
	if(f->flag==FUTURE_QUEUE){
		if(f->state==FUTURE_EMPTY){
			f->state=FUTURE_WAITING;
			en_queue(f->get_q,currpid);
			i=d_queue(f->set_q);
			if(i!=-1){
				p=get_q(f->set_q);
				restore(mask);
				ready(p);
				suspend(currpid);
			} else {
			  suspend(currpid);
			}
		}
		if(f->state==FUTURE_WAITING){
			en_queue(f->get_q,currpid);
			i=d_queue(f->set_q);
			if(i!=-1){
				p=get_q(f->set_q);
				restore(mask);
				suspend(currpid);
			} else {
				suspend(currpid);
			}

		}
		if(f->state==FUTURE_VALID){		
			*value=*(f->value);
			
			i=d_queue(f->set_q);
			if(i!=-1){
			   if(isempty_q(f->get_q)) {
			      f->state = FUTURE_EMPTY;
			   } else {
				f->state=FUTURE_WAITING;
				p=get_q(f->set_q);
				ready(p);
			   }
			}
			else{if (isempty_q(f->get_q)) {
				   f->state = FUTURE_EMPTY; 
				} else {
				   f->state = FUTURE_WAITING;
				}
			}
			restore(mask);
			return OK;
		}
	}
	if(f->flag==FUTURE_EXCLUSIVE){
		if(f->state==FUTURE_VALID){
			f->state=FUTURE_EMPTY;
			*value=*(f->value);
			restore(mask);
			return OK;
			
		}
		else
		{
			restore(mask);
			return SYSERR;
		}
	}
	return SYSERR;

/*	if(f->flag == FUTURE_EXCLUSIVE)
		{	d = getpid();
			if(f->state == FUTURE_EMPTY)
			{
				suspend(f->pid);
			}
			*value = f->value ;
			f->state = FUTURE_EMPTY;
		}
		else if(f->flag == FUTURE_SHARED)
		{
			if(f->state == FUTURE_EMPTY)
			{
			
				enqueueFuture(&(f->get_queue),getpid());
				suspend(getpid());
			}
			*value = f->value;
		}
		else if (f->flag == FUTURE_QUEUE)
		{
	        	if(f->set_queue.size == 0)
			{
			    enqueueFuture(&(f->get_queue), getpid());
			    suspend(getpid());
			}
			else
			{
			    enqueueFuture(&(f->get_queue), getpid());
			    resume(dequeueFuture(&(f->set_queue)));
			    
			    while(f->state != FUTURE_VALID)
			    {
			        resched();
			    } 
			} 
	        	*value = f->value;
	        	f->state = FUTURE_EMPTY; 
	    	}
		else
		{
			kprintf("Future State not recognized");
			return SYSERR;
		}
		return OK
   



*	if(fut->state == FUTURE_VALID){
		fut->state = FUTURE_EMPTY;
		*val = *(fut->value);
		//printf("Value in future to get %d\n",*(fut->value));
		//printf("Value that we got is %d\n", *val);
		return OK;
	}
	else if(fut->state == FUTURE_EMPTY) {
		//printf("Suspending process %d\n",getpid());
		fut->state = FUTURE_WAITING;
		fut->pid = getpid();
		suspend(getpid());
		*val = *(fut->value);
		fut->state = FUTURE_EMPTY;
		//printf("Value in future to get %d\n", *(fut->value));
		return OK;
	}
	else{
		return SYSERR;
	}*/
}
