#include <future.h>

syscall future_get(future *f, int *value){

	if(f->state == FUTURE_VALID){
		*value = f->value;
		f->state = FUTURE_EMPTY;
		return OK;
	}
	else if(f->state == FUTURE_EMPTY){
		f->state = FUTURE_WAITING;
		return OK;
	}
	return SYSERR;
}