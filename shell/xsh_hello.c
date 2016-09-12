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

