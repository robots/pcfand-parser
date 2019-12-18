#ifndef MAIN_h_
#define MAIN_h_


#ifdef WIN32
#define err(x, args...) do { fprintf(stderr, args); exit(x); } while (0);
#else
#include <err.h>
#endif

#endif
