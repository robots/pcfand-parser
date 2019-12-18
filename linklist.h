#ifndef LINKLIST_h
#define LINKLIST_h


#include <stdlib.h>
#include <stdio.h>

#include "types.h"

enum {
	TYPE_F='F',
	TYPE_N='N',
	TYPE_A='A',
	TYPE_Z='Z',
	TYPE_D='D',
	TYPE_B='B',
	TYPE_T='T',
	TYPE_H='H'
};

struct tNode {
    char ID;
    char *Name;
    struct pData *Data;    
    int (*Command)(FILE*, struct pData *, FILE *);
    struct tNode *Next;
};

char *get_token(FILE *);

void ll_id(void);
void ll_walkthrough(FILE *, FILE *);
int ll_fill(FILE* fpin);
void ll_destroy(void);

#endif
