#include<future.h>

syscall future_set(future* f, int* value){
	intmask mask;
	pid32 process_id;
	int i;
	mask = disable();
	if(f->flag==FUTURE_EXCLUSIVE){
		if(f->state==FUTURE_WAITING || f->state==FUTURE_EMPTY){
			f->state=FUTURE_VALID;
			f->value=value;
			restore(mask);
			return OK;
		}
		else
		{
			restore(mask);
			return SYSERR;
		}

	}
	if(f->flag==FUTURE_SHARED){
		if (f->state==FUTURE_WAITING || f->state==FUTURE_EMPTY){
			f->state=FUTURE_VALID;
			f->value=value;		
			while(1)
			{
				i=d_queue(f->get_q);
				if(i==-1)
					break;
				process_id=get_q(f->get_q);
				resume(process_id);
			}
			
			restore(mask);
			return OK;
		}
		else
		{
			restore(mask);
			return SYSERR;
		}
	}
	if(f->flag==FUTURE_QUEUE)
	{
				if(f->state==FUTURE_EMPTY || f->state==FUTURE_VALID)
				{
					en_queue(f->set_q,currpid);
					restore(mask);
					suspend(currpid);
}
				if(f->state==FUTURE_WAITING)
				{
					f->state=FUTURE_VALID;
					f->value=value;
					i=d_queue(f->get_q);
					if(i!=-1)
					{
						process_id=get_q(f->get_q);
						
					        restore(mask);
						resume(process_id);
						return OK;						
					}
					restore(mask);
					return OK;
				}
				else{
					restore(mask);
					return SYSERR;
				}
	}
	return OK;


 /* f->value = val;
        f->state = FUTURE_VALID;
	if(f->flag == FUTURE_EXCLUSIVE)
	{
		f->value = *value;
		f->state = FUTURE_VALID;
		resume(f->pid);
	}
	else if (f->flag == FUTURE_SHARED)
	{
		if(f->state == FUTURE_VALID)
		{
			return SYSERR;
		}
		f->value = *value;
		f->state = FUTURE_VALID;
		
		int i;
		for(i=0; i<f->get_queue.size; i++)
		{
			resume(f->get_queue.p[i]);
		}
	}
	else if(f->flag == FUTURE_QUEUE)
	{
		if(f->get_queue.size ==0)
		{
			enqueueFuture(&(f->set_queue), getpid());
			suspend(getpid());
		}
		
		while(f->state == FUTURE_VALID)
		{
			resched();
		}
		f->value = *value;
		f->state = FUTURE_VALID;
		resume(dequeueFuture(&f->get_queue));
	}
	return OK;
	

         *if(fut->state==FUTURE_EMPTY ) {
		*(fut->value) = *val;
		//printf("Value in future we set is %d\n",*(fut->value));
		//printf("Value sent to to set is %d\n", *val);
		fut->state=FUTURE_VALID;
		return OK;
	}

	else if(fut->state==FUTURE_WAITING) {
		//printf("Resuming process %d\n",fut->pid);
		*(fut->value) = *val;
		resume(fut->pid);
		//printf("Value in future we set is %d\n",*(fut->value));
		//printf("value sent to to set is %d\n", *val);
		fut->state=FUTURE_VALID;
		return OK;
	}
	else
	{
		return SYSERR;
	}*/
}
