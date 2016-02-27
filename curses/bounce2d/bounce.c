#include <curses.h>
#include <stdio.h>
#include "bounce.h"
#include <sys/time.h>
#include <signal.h>

struct ppball the_ball;

void init();
int set_ticker(int);
void ball_move(int);           //SIGALRM handler
int bounce_or_lose(struct ppball*);   //到达四个边界时，注意反弹
void wrap_up();

int main(){
	int c;

	init();

	while((c = getchar()) != 'q'){
		if (c == 'f') the_ball.x_ttm--;
		if (c == 'F') the_ball.y_ttm--;
		if (c == 's') the_ball.x_ttm++;
		if (c == 'S') the_ball.y_ttm++;
	}

	wrap_up();
	return 0;
}


void init(){
	
	the_ball.x_pos = X_INIT;
	the_ball.y_pos = Y_INIT;
	the_ball.x_ttg = the_ball.x_ttm = X_TTM;
	the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
	the_ball.x_dir = 1;
	the_ball.y_dir = 1;
	the_ball.symbol = DEF_SYMBOL;

	initscr();
	noecho();
	crmode();

	signal(SIGINT, SIG_IGN);

	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
	move(LINES -1, COLS -1);
	refresh();

	signal(SIGALRM, ball_move);
	set_ticker(1000/TICKS_PER_SEC);
}

void ball_move(int signum){

	int x_cur, y_cur, moved;
	signal(SIGALRM, SIG_IGN);

	moved = 0;
	x_cur = the_ball.x_pos;
	y_cur = the_ball.y_pos;

	if(the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1){
		the_ball.x_pos += the_ball.x_dir;
		the_ball.x_ttg = the_ball.x_ttm;
		moved = 1;
	}

	if(the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1){
		the_ball.y_pos += the_ball.y_dir;
		the_ball.y_ttg = the_ball.y_ttm;
		moved = 1;
	}
	
	if( moved ){
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);

		bounce_or_lose(&the_ball);
		move(LINES - 1, COLS -1);
		refresh();
	}
	signal(SIGALRM, ball_move);

}

int bounce_or_lose(struct ppball * bp){

	int ret = 0;

	if (bp->y_pos <= TOP_ROW){
		the_ball.y_dir =  1;
		ret = 1;	
	}
	if (bp->y_pos >= BOT_ROW){
		the_ball.y_dir = -1;
		ret = 1;
	}
	if (bp->x_pos <= LEFT_EDGE){
		the_ball.x_dir = 1;
		ret = 1;
	}
	if (bp->x_pos >= RIGHT_EDGE){
		the_ball.x_dir = -1;
		ret = 1;
	}

	return ret;
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

void wrap_up(){
	set_ticker(0);
	endwin();
}
