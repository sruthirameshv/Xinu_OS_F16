#include <xinu.h>
#include <prodcons.h>
#include <stdlib.h>
#include <string.h>

int n;   //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

sid32 produced, consumed;

shellcmd xsh_prodcons(int nargs, char *args[])
{
  produced = semcreate(0);
  consumed = semcreate(1);
  //Argument verifications and validations
  int count = 2000;             //local varible to hold count
  
  //check args[1] if present assign value to count
  if(nargs == 2)
  {
    if(strcmp(args[1],"-f") == 0){
      future *f1, *f2, *f3;
 
      f1 = future_alloc(FUTURE_EXCLUSIVE);
      f2 = future_alloc(FUTURE_EXCLUSIVE);
      f3 = future_alloc(FUTURE_EXCLUSIVE);
 
      resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
      resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
      resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
      resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );
      resume( create(future_cons, 1024, 20, "fcons3", 1, f3) );
      resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );

      future_free(f1);
      future_free(f2);
      future_free(f3);

      return 0;
    }
    else if(atoi(args[1]) == 0)
    {
     fprintf(stderr, "%s: Please enter a valid number\n", args[0]);
     return 1; 
    }
    else
    {
     count  = atoi(args[1]); 
    }
  }
  else if(nargs > 2) 
  {
    fprintf(stderr, "%s:Too many arguments\n", args[0]);
    return 1;
  }
  //create the process producer and consumer and put them in ready queue.
  //Look at the definations of function create and resume in the system folder for reference.      
  resume( create(producer, 1024, 20, "producer", 1, count));
  resume( create(consumer, 1024, 20, "consumer", 1, count));
  return 0;
}
