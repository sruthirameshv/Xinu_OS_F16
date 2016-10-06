#include <xinu.h>

uint future_cons(future *fut) {
	int i, j, status;
	for(i=0; i<5; i++) {
		status = future_get(fut, &j);
		if(status < 1){

		}
		else{
			printf("Value consumed is: %d \n",j);
		}
	}
	return OK;
}