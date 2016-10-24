#include <xinu.h>

syscall future_free(future* fut) {
        int status;
        status = freemem((char *)fut,sizeof(future));
        if(status < 1){
            return SYSERR;
	}
        return OK;
}
/*	deallocateQueue(&(f->set_queue));
	deallocateQueue(&(f->get_queue));
	return freemem(fut, sizeof(future));
}*/
