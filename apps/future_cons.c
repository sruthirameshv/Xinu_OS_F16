//#include <xinu.h>
#include<future.h>
typedef unsigned int         uint;
uint future_cons(future *fut) {

  int i, status;
  status = future_get(fut, &i);
  if (status < 1) {
    kprintf("future_get failed\n");
    return -1;
  }
  kprintf("it consumed %d\n", i);
  if(fut->flag == FUTURE_EXCLUSIVE || isempty_q(fut->get_q)){
    future_free(fut);
  }
  return OK;
}
