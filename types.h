#ifndef TYPES_h
#define TYPES_h

#include <stdio.h>
#include <stdlib.h>

struct pData {
	void *data;
	char *db_col;
};

struct tFloat {
    int total;
    int prec;
};


int parse_N(FILE *fp, struct pData *, FILE *);
int parse_A(FILE *fp, struct pData *, FILE *);
int parse_Z(FILE *fp, struct pData *, FILE *);
int parse_F(FILE *fp, struct pData *, FILE *);
int parse_D(FILE *fp, struct pData *, FILE *);
int parse_B(FILE *fp, struct pData *, FILE *);
int parse_T(FILE *fp, struct pData *, FILE *);
int parse_H(FILE *fp, struct pData *, FILE *);

void trim_space(char *);

#endif
