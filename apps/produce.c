#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
	int i =1;
	while(i<=count){
		wait(consumed);
		n=i;
		printf("Value of n- Produce side :\t%d\n",n);
		signal(produced);
		i++;
	}
}
