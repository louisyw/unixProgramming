#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smshl.h"

#define is_delim(x) ((x) == ' ' || (x) == '\t')
char *next_cmd(char * prompt, FILE * fp){
	int c;
	int pos = 0;      //store the position of current buf
	int bufspace = 0;  //SIZE of buf
	char *buf;

	printf("%s", prompt);
	while((c= getc(fp)) != EOF){
		if (pos + 1 >= bufspace){
			if (bufspace == 0){
				buf = emalloc(BUFSIZ);   //define in stdlib.h  =8192
			}else{
				buf = erealloc(buf, bufspace + BUFSIZ);
			}
			bufspace += BUFSIZ;
		}

		/*end of command*/
		if (c == '\n'){
			break;
		}
		/*no, add to buf*/
		buf[pos++] = c;
	}

	if (c == EOF && pos == 0){    //EOF and no input
		return NULL;
	}
	buf[pos] = '\0';
	return buf;
}

char ** splitline(char *line){
	char *newstr();
	char **args;   //store for arg
	int spots = 0;        //spots in table
	int bufspace = 0;	//bytes in table
	int argnum = 0;		//slots used
	char *cp = line;    //pos in string
	char *start;
	int len;

	if (line == NULL){
		return NULL;                 //nothing
	}

	args = emalloc(BUFSIZ);
	bufspace = BUFSIZ;
	spots = BUFSIZ/sizeof(char *);    //BUFSIZ bytes   char = 2 bytes

	while(*cp != '\0'){
		while(is_delim(*cp))   //skip leading space
			cp++;
		if ( *cp == " \0 ")      //quit at end of string,空字符串
			break;

		/*make sure the array has enough space*/
		if (argnum + 1 > spots){
			args = erealloc(args, bufspace+BUFSIZ);
			bufspace += BUFSIZ;
			spots += (BUFSIZ / sizeof(char *) );
		}

		/*make start, then find end of word*/
		start = cp;
		len = 1;    //跳过检验*cp
		while(*++cp != '\0' && !(is_delim(*cp)) )
			len++;
		args[argnum++] = newstr(start, len);
	}
	args[argnum] = NULL;
	return args;
}

char *newstr(char *s, int l){
	char *rv = emalloc(l+1);

	rv[l] = '\0';

	strncpy(rv, s, l);

	return rv;
}


void freelist(char **list){
	char **cp = list;

	while(*cp){
		free(*cp++);
	}

	free(list);
}

void *emalloc(size_t n){
	void *rv;

	if((rv = malloc(n)) == NULL)
		fatal("out of memory", "", 1);
	return rv;
}

void *erealloc(void* p, size_t n){
	void *rv;

	if((rv = realloc(p, n)) == NULL)
		fatal("out of memory", "", 1);
	return rv;
}
