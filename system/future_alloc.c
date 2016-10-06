#include <xinu.h>

future* future_alloc(int future_flags) {
	future *fut;
	fut = (future *)getmem(sizeof(future));
	fut->value = (int*)getmem(sizeof(int));
	fut->flag = future_flags;
	fut->state = FUTURE_EMPTY;
	return fut;
}