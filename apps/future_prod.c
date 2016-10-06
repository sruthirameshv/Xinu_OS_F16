#include <xinu.h>

uint future_prod(future *fut){
	int i, j;
	j=10;
	for(i=0; i<6; i++){
		j+=i;
		printf("Produced value is: %d\n",j);
		future_set(fut, &j);
	}
	return OK;
}