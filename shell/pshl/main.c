#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define MAXARGS 20   //cmdline args
#define ARGLEN 100   //Token length

char * makestring(char *);
int execute(char *arglist[]);
int main(){
	char *argList[MAXARGS + 1];   //the last for NULL
	int numargs = 0;
	char argbuf[ARGLEN];

	while(numargs < MAXARGS){
		if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n'){
			argList[numargs++] =  makestring(argbuf);
		}else{
			if(numargs > 0){
				argList[numargs] = NULL;
				execute(argList);
				numargs = 0;
			}
		}
	}
	return 0;
}

char *makestring(char *buf){
	char *cp;

	buf[strlen(buf) - 1] = '\0';

	cp = malloc(strlen(buf) +1);

	if(cp == NULL){
		fprintf(stderr, "no memory\n");
		exit(1);
	}

	strcpy(cp , buf);

	return cp;
}

int execute(char *arglist[]){
	if(execvp(arglist[0], arglist) == -1){
		perror("execvp");
		exit(1);
	}
}
