<<<<<<< HEAD
/*xsh_hello.c - xshhello*/
#include <xinu.h>
#include <stdio.h>
#include <string.h>
/*------------------------------------------------------------------------
 * xsh_cat - shell command to cat one or more files
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]){
	if (nargs == 2){
           printf("Hello %s, Welcome to the world of Xinu!!", args[1]);
		return 1;
	}
	else if( nargs > 2 ) {
      		printf("Too many arguments supplied.\n");
   		
	}
	else {
      		printf("One argument expected.\n");

	
	return 0;
}}
=======
/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

shellcmd xsh_hello(int nargs, char *args[]){
	if(nargs >= 3){
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		return 1;
	}
	else if (nargs < 2){
		fprintf(stderr, "%s: too few arguments\n", args[0]);
		return 1;
	}
	else {
		printf("Hello %s, Welcome to the world of Xinu!\n", args[1]);
	}
	return 0;
}

>>>>>>> 8fd62a37dfa17b34d0f27731f06b5650ee27ec28

