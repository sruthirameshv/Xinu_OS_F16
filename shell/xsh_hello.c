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