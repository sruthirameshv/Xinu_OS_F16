#include <xinu.h>

syscall future_get(future *fut, int* val){

	if(fut->state == FUTURE_VALID){
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
	}
}