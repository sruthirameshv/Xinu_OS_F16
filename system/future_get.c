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
		} else if(futureVar->state == FUTURE_EMPTY || futureVar->state == FUTURE_WAITING) {
			printf("suspending process %d\n",getpid());
			futureVar->state = FUTURE_WAITING;
			curpid = getpid();
			queue_add(&futureVar->get_queue, curpid);
			suspend(curpid);
			*val = *(futureVar->value);
			futureVar->state = FUTURE_EMPTY;
			return OK;
		}
	} else { // For FUTURE_QUEUE

		if(futureVar->set_queue.count > 0) {
				curpid = queue_remove(&futureVar->set_queue);
				printf("resuming producer process %d\n",curpid);
				resume(curpid);
				*val = *(futureVar->value);
		} else {
			queue_add(&futureVar->get_queue, getpid());
			printf("suspending consumer process %d\n",getpid());
			suspend(getpid());
			*val = *(futureVar->value);
		}
		return OK;
	}
}

syscall future_get_str(future *futureVar, int* val, char* strval){
	pid32 curpid;
	if(futureVar->flag == FUTURE_EXCLUSIVE) {
		if(futureVar->state == FUTURE_VALID){
			*val = *(futureVar->value);
			memcpy(strval, futureVar->str_val, *val);
			printf("FUTURE GET STRING: %s\n", strval);

			futureVar->state = FUTURE_EMPTY;
			return OK;
		} else if(futureVar->state == FUTURE_EMPTY) {
			printf("suspending process %d\n",getpid());
			futureVar->state = FUTURE_WAITING;
			futureVar->pid = getpid();
			suspend(getpid());
			*val = *(futureVar->value);
			memcpy(strval, futureVar->str_val, *val);
			printf("FUTURE GET STRING: %s\n", strval);
			
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
		} else if(futureVar->state == FUTURE_EMPTY || futureVar->state == FUTURE_WAITING) {
			printf("suspending process %d\n",getpid());
			futureVar->state = FUTURE_WAITING;
			curpid = getpid();
			queue_add(&futureVar->get_queue, curpid);
			suspend(curpid);
			*val = *(futureVar->value);
			futureVar->state = FUTURE_EMPTY;
			return OK;
		}
	} else { // For FUTURE_QUEUE

		if(futureVar->set_queue.count > 0) {
				curpid = queue_remove(&futureVar->set_queue);
				printf("resuming producer process %d\n",curpid);
				resume(curpid);
				*val = *(futureVar->value);
		} else {
			queue_add(&futureVar->get_queue, getpid());
			printf("suspending consumer process %d\n",getpid());
			suspend(getpid());
			*val = *(futureVar->value);
		}
		return OK;
	}
}
