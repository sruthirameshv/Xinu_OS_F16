/*Global variable for producer consumer*/
#include<xinu.h>
extern int n; /*this is just declaration*/
extern sid32 produced, consumed;
/*function Prototype*/
void consumer(int count);
void producer(int count);

/* Semaphore Declarations */
