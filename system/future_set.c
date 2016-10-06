#include <xinu.h>

syscall future_set(future *fut, int* val){

	if(fut->state==FUTURE_EMPTY ) {
		*(fut->value) = *val;
		printf("Value in future we set is %d\n",*(fut->value));
		printf("Value sent to to set is %d\n", *val);
		fut->state=FUTURE_VALID;
		return OK;
	}

	else if(fut->state==FUTURE_WAITING) {
		printf("Resuming process %d\n",fut->pid);
		*(fut->value) = *val;
		resume(fut->pid);
		printf("Value in future we set is %d\n",*(fut->value));
		printf("value sent to to set is %d\n", *val);
		fut->state=FUTURE_VALID;
		return OK;
	}
	else
	{
		return SYSERR;
	}
}