#include <xinu.h>

syscall future_get(future *futureVar, int* val){
	pid32 curpid;
	if(futureVar->flag == FUTURE_EXCLUSIVE) {
		if(futureVar->state == FUTURE_VALID){
			*val = *(futureVar->value);
			futureVar->state = FUTURE_EMPTY;
			return OK;
		} else if(futureVar->state == FUTURE_EMPTY) {
			printf("suspending process %d\n",getpid());
			futureVar->state = FUTURE_WAITING;
			futureVar->pid = getpid();
			suspend(getpid());
			*val = *(futureVar->value);
			futureVar->state = FUTURE_EMPTY;
			return OK;
		} else {
			return SYSERR;
		}
	} else if(futureVar->flag == FUTURE_SHARED) {
		if(futureVar->state == FUTURE_VALID){
			*val = *(futureVar->value);
			futureVar->state = FUTURE_EMPTY;
			return OK;
		} else if(futureVar->state == FUTURE_EMPTY) {
			printf("suspending process %d\n",getpid());
			futureVar->state = FUTURE_WAITING;
			curpid = getpid();
			queue_add(&futureVar->get_queue, curpid);
			suspend(curpid);
			*val = *(futureVar->value);
			futureVar->state = FUTURE_EMPTY;
			return OK;
		} else if(futureVar->flag == FUTURE_QUEUE) {

			return SYSERR;
		}
	} else { // For FUTURE_QUEUE

		if(futureVar->set_queue.count > 0) {
				curpid = queue_remove(&futureVar->set_queue);
				resume(curpid);
				*val = *(futureVar->value);
		} else {
			queue_add(&futureVar->get_queue, getpid());
			suspend(getpid());
			*val = *(futureVar->value);
		}
		return OK;
	}
}
