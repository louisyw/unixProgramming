#ifndef UTMPLIB_H_
#define UTMPLIB_H_

#define  NRECS 16
#define NULLUT ((struct utmp*) NULL)
#define UTSIZE (sizeof(struct utmp))

int utmp_open(const char * filename);

struct utmp * utmp_next();   //record the printed struct

int utmp_reload();    // reload the buffer
void utmo_close();

#endif
