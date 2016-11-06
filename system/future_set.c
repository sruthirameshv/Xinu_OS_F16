#include <xinu.h>

	syscall future_set(future* myFuture, int* val) 
	{	
		pid32 curpid;

		if(myFuture->flag == FUTURE_QUEUE ) {
			if(myFuture->get_queue.count > 0) {
				*(myFuture->value) = *val;
				curpid = queue_remove(&myFuture->get_queue);
				resume(curpid);
			} else {
				queue_add(&myFuture->set_queue, getpid());
				suspend(getpid());
				*(myFuture->value) = *val;
			}
			return OK;
		}


		if(myFuture->state==FUTURE_EMPTY ) {
			*(myFuture->value) = *val;
			myFuture->state=FUTURE_VALID;
			return OK;
		}
		else if(myFuture->state==FUTURE_WAITING) {
			printf("resuming process %d\n",myFuture->pid);
			*(myFuture->value) = *val;
			myFuture->state=FUTURE_VALID;

			if(myFuture->flag == FUTURE_SHARED) {
				curpid = queue_remove(&myFuture->get_queue);
				while(curpid != SYSERR) {
					resume(curpid);
				}
			} else if(myFuture->flag == FUTURE_QUEUE) {
				curpid = queue_remove(&myFuture->get_queue);
				resume(curpid);
			} else {
				resume(myFuture->pid);
			}
			return OK;
		}
		 else
		{	
			return SYSERR;

		}
		
	}
