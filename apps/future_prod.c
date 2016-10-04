#include <future.h>
typedef unsigned int 	uint;

uint future_prod(future *fut){
	int i,j;
	j = (int)fut;

	for(int i=0; i<1000; i++){
		j+=i;
	}

	if(future_set(fut,j) == SYSERR){
		printf("THE PRODUCED VALUE IS NOT CONSUMED YET");
	}
	return OK;
}