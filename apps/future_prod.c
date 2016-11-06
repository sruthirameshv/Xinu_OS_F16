#include <xinu.h>

uint future_prod(future *fut) {
  int i, j;
  j = 10;
  i = rand();
  printf("Produced %d\n",i);
  future_set(fut, &i);
  return OK;
}