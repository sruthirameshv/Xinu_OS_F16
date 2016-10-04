#include <future.h>
typedef unsigned int 	uint;

uint future_cons(future *fut){
	int i, status;
	
	while(TRUE){
		
		status = future_get(fut, &i);

		if(status == OK && fut->state == FUTURE_EMPTY){
			printf("Value consumed is %x", i);
			break;
		}
		else if (status == OK){
			continue;
		}
		if(status == SYSERR){
			continue;
		}
	}
	return OK;
}