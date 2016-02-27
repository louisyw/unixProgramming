/*
 *read and print 24 lines
 */
#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more();

int main(int argc, char *argv[]){
	FILE * fp;

	if (argc == 1){
		do_more(stdin);
	}else{
		while(--argc){
			if ((fp = fopen(argv[1], "r")) != NULL){
				do_more(fp);
				fclose(fp);
			}else exit(1);
		}
	}
	return 0;
}

void do_more(FILE * fp){
	char line[LINELEN];

	int num_of_lines = 0;
	int see_more(), reply;

	while(fgets(line, LINELEN, fp)){
		if (num_of_lines == PAGELEN){
			if((reply = see_more() ) == 0){
				break;
			}
			num_of_lines -= reply;
		}

		if (fputs(line, stdout) == EOF) exit(1);
		num_of_lines++;
	}
}

int see_more(){
	int c;
	printf("\033[7m more?\033[m");

	while((c=getchar()) != EOF){
		if (c == 'q')
			return 0;
		if (c == ' ')
			return PAGELEN;
		if (c == '\n')
			return 1;
	}
}

