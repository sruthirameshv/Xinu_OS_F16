#include <xinu.h>

	syscall future_set(future* myFuture, int* val) 
	{	
		pid32 curpid;

		if(myFuture->flag == FUTURE_QUEUE ) {
			if(myFuture->get_queue.count > 0) {
				*(myFuture->value) = *val;
				curpid = queue_remove(&myFuture->get_queue);
				printf("resuming consumer process %d\n",curpid);
				resume(curpid);
			} else {
				queue_add(&myFuture->set_queue, getpid());
				printf("resuming producer process %d\n",getpid());
				suspend(getpid());
				*(myFuture->value) = *val;
			}
			return OK;
		}

		if(myFuture->state==FUTURE_EMPTY ) {
			*(myFuture->value) = *val;
			myFuture->state=FUTURE_VALID;
			return OK;
		} else if(myFuture->state==FUTURE_WAITING) {
			*(myFuture->value) = *val;
			myFuture->state=FUTURE_VALID;

			if(myFuture->flag == FUTURE_SHARED) {
				curpid = queue_remove(&myFuture->get_queue);
				while(curpid != SYSERR) {
					resume(curpid);
					printf("resuming process %d\n",curpid);
					curpid = queue_remove(&myFuture->get_queue);
				}
			} else if(myFuture->flag == FUTURE_QUEUE) {
				curpid = queue_remove(&myFuture->get_queue);
				resume(curpid);
				printf("resuming process %d\n",curpid);
			} else {
				resume(myFuture->pid);
			}
			return OK;
		} else {	
			return SYSERR;
		}
		
	}

	syscall future_set_str(future* myFuture, int* val, char* strval) 
	{	
		pid32 curpid;

		if(myFuture->flag == FUTURE_QUEUE ) {
			if(myFuture->get_queue.count > 0) {
				*(myFuture->value) = *val;
				curpid = queue_remove(&myFuture->get_queue);
				printf("resuming consumer process %d\n",curpid);
				resume(curpid);
			} else {
				queue_add(&myFuture->set_queue, getpid());
				printf("resuming producer process %d\n",getpid());
				suspend(getpid());
				*(myFuture->value) = *val;
			}
			return OK;
		}

		if(myFuture->state==FUTURE_EMPTY ) {
			*(myFuture->value) = *val;
			// STring copy
			// printf("FUTURE SET STRING\n");
			myFuture->str_val = (char *)getmem(*val * sizeof(char));
			memcpy(myFuture->str_val, strval, *val);
			printf("FUTURE SET STRING: %s\n", myFuture->str_val);

			myFuture->state=FUTURE_VALID;
			return OK;
		} else if(myFuture->state==FUTURE_WAITING) {
			*(myFuture->value) = *val;
			myFuture->state=FUTURE_VALID;

			if(myFuture->flag == FUTURE_SHARED) {
				curpid = queue_remove(&myFuture->get_queue);
				while(curpid != SYSERR) {
					resume(curpid);
					printf("resuming process %d\n",curpid);
					curpid = queue_remove(&myFuture->get_queue);
				}
			} else if(myFuture->flag == FUTURE_QUEUE) {
				curpid = queue_remove(&myFuture->get_queue);
				resume(curpid);
				printf("resuming process %d\n",curpid);
			} else {
				resume(myFuture->pid);
			}
			return OK;
		} else {	
			return SYSERR;
		}
		
	}
