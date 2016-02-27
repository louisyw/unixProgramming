#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#define MESSAGE "Hello"
#define BLANK "     "

int row;    //current row
int col;     //current column
int dir;    //where we are going

void move_msg(int);    //alrm handler
int set_ticker(int);


int main(){
	int delay;
	int ndelay;

	int c;

	initscr();
	crmode();
	noecho();
	clear();

	row = 10;
	col = 0;
	dir = 1;
	delay = 200;

	move(row, col);
	addstr(MESSAGE);
	signal(SIGALRM, move_msg);
	set_ticker(delay);
	move(LINES -1, COLS -1);
	refresh();

	while(true){
		ndelay = 0;

		c= getch();
		if(c == 'q') break;
		if(c == ' ') dir = -dir;
		if(c == 'f' && delay > 2 ) ndelay = delay /2;
		if(c == 's')  ndelay = delay *2;
		if(ndelay > 0){
			set_ticker(delay = ndelay);
		}
	}

	endwin();
	return 0;
}

void move_msg(int signum){
	signal(SIGALRM, move_msg);
	move(row, col);
	addstr(BLANK);
	col += dir;
	move(row, col);
	addstr(MESSAGE);
	move(LINES -1,COLS -1);
	refresh();

	if(dir == -1 && col <= 0)
		dir = 1;
	else if(dir ==1 && col+strlen(MESSAGE) >= COLS)
		dir = -1;
}

int set_ticker(int n_msec){
        struct itimerval new_timeset;
	long n_sec, n_usec;

	n_sec = n_msec / 1000;
	n_usec = (n_msec % 1000 )*1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usec;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usec;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}  
