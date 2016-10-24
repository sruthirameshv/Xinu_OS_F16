//#include <xinu.h>
#include <future.h>
typedef unsigned int         uint;
/*uint future_prod(future *fut) {
  int i, j=0;
  j = (int)fut;
  for (i=0; i<1000; i++) {
    j += i;
  }
  future_set(fut, &j);
  return OK;
}*/
uint future_prod(future *fut)
{
  int i, j, status;
  j = (int)fut;

  for (i=0; i<1000; i++) {
    j += i;
  }
  kprintf("produced %d\n",j);
  status=future_set(fut, &j);
  if (status < 1)
  {
    kprintf("future_set failed\n");
    return -1;
  }
  return OK;
}
