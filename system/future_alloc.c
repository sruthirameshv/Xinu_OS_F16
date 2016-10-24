//#include <xinu.h>
#include<future.h>
future* future_alloc(int future_flag) {
	future *p;
	p = (future *)getmem(sizeof(future));
       if (p!=NULL){
			p->flag=future_flag;
			p->state=FUTURE_EMPTY;
			p->set_q=init_q();
			p->get_q=init_q();
		}
		return p;	
/*fut->value = (int*)getmem(sizeof(int));
	fut->flag = future_flags;
	int status1 = initializeQueue(&(obj->set_queue), 100);
	int status2 = initializeQueue(&(obj->get_queue), 100);
		
		if(status1 == -1 || status2 == -1)
		{
			freemem((char*)obj, sizeof(obj));
			kprintf("Queue cannot be created");
			return NULL;
		}

	fut->state = FUTURE_EMPTY;
	return fut;*/
}
