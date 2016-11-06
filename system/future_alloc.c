#include <xinu.h>

future* future_alloc(int future_flags){
	future *myFuture;
	myFuture = (future *)getmem(sizeof(future));
	myFuture->value = (int*)getmem(sizeof(int));
	myFuture->flag = future_flags;
	myFuture->state = FUTURE_EMPTY;

	if(future_flags == FUTURE_SHARED) {
		//myFuture->get_queue = (queue *)getmem(sizeof(queue));
		queue_init(&myFuture->get_queue);
	} else if(future_flags == FUTURE_QUEUE) {
		//myFuture->get_queue = (queue *)getmem(sizeof(queue));
		queue_init(&myFuture->get_queue);
		//myFuture->set_queue = (queue *)getmem(sizeof(queue));
		queue_init(&myFuture->set_queue);
	}
	return myFuture;

}
