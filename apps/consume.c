#include <xinu.h>
#include <prodcons.h>

void consumer(int count) {
	int i =1;
	while(i<=count){
		wait(produced);
		printf("Consumer consumed:%d \n",n);
		signal(consumed);
		i++;
	}
}

