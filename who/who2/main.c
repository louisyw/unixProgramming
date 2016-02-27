/*
 *who - read /etc/utmp and list info therein
 */
#include <time.h>
#include <stdio.h>
#include <utmp.h>
#include "utmplib.h"

void showtime(long);
void show_info(struct utmp *);

int main(){	
	struct utmp * temp;
	utmp_open(UTMP_FILE);

	while((temp = utmp_next()) != NULLUT)
		show_info(temp);
	
	utmp_close();
	return 0;
}



void show_info(struct utmp * utbufp){
	if (utbufp->ut_type != USER_PROCESS)
		return;
	
	printf("%-8.8s", utbufp->ut_name);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
	showtime(utbufp->ut_time);

#ifdef SHOWHOST
	if(utbufp->ut_host[0] != '\0')
		printf("(%s)", utbufp->ut_host);
#endif
	printf("\n");
}


void showtime(long timeval){
	char *cp;
	cp = ctime(&timeval);

	printf("%12.12s", cp+4);
}
