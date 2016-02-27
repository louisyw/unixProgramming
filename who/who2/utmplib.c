/*
 *一次read（系统调用）出<=16个结构体，num_rec表明缓冲区中读到的结构体数量
 *cur_rec表明已经使用过的结构体
 */

#include <unistd.h>
#include <utmp.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include "utmplib.h"


static char utmpbuf[UTSIZE * NRECS];   //buffer
static int utmp_fd = -1;
static int cur_rec;                    //used
static int num_rec;			//the total number of utmp in buffer

int utmp_open(const char *filename){
	utmp_fd = open(filename, O_RDONLY);
	if (utmp_fd == -1){
		exit(1);
	}
	cur_rec = num_rec = 0;
	return utmp_fd;
}

/*
 *reload the buffer if cur_rec == num_rec
 *
 */
int utmp_reload(){
	int amt_read;
	amt_read = read(utmp_fd, utmpbuf, (UTSIZE*NRECS));
	num_rec = amt_read/UTSIZE;
	cur_rec = 0;

	return num_rec;
}

struct utmp * utmp_next(){
	struct utmp *recp;
	/*utmp_reload() count on cur_rec == num_rec*/
	if( cur_rec == num_rec && utmp_reload() == 0)  
		return NULLUT;
	
	recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
	cur_rec++;

	return recp;
}


void utmp_close(){
	close(utmp_fd);
}
