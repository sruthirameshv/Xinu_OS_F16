#include <future.h>

syscall future_free(future *f){
	freemem(f, sizeof(future));
	return OK;
}