#define _XOPEN_SOURCE 700

#include <string.h>


#include "tr.h"


char *tr_tab[] = {

	[0xa9] = "e",
}

char *tr_K(char *in_buf)
{
	char *tmp;
	char *out;
	int i;
	int len;
	int idx;

	len = strlen(in_buf);

	tmp = malloc(len);
	if (tmp == NULL) {
		err(1, "error allocating memory\n");
	}

	idx = 0;
	for (i = 0; i < len; i++) {
		char ch;

		ch = in_buf[i];
		strcpy(tmp+idx, tr_tab[ch]);
		idx += strlen(tr_tab[ch]);
	}

	out = strdup(tmp);
	free(tmp);

	return out;
}

