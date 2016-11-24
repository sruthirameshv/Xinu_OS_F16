#include <xinu.h>

uint future_cons(future *fut) {

  int i, status,j;
  status = future_get(fut, &j);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("it consumed %d\n", j);
  return OK;
}
