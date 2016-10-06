#include <xinu.h>

syscall future_free(future* fut) {
	return freemem(fut, sizeof(future));
}