#define _XOPEN_SOURCE 700

#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "main.h"
#include "t00.h"


FILE *t00_fp = NULL;

#define BLOCK_SIZE 0x200
#define DATA_SIZE  ((BLOCK_SIZE)-2)

#undef DEBUG
//#define DEBUG 1
/*
void t00_size(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	size_t filesize = ftell(fp);
	fprintf(stderr, "T00 size = %d\n", filesize);
	fseek(fp, 0, SEEK_SET);
}
*/

int t00_init(const char *file)
{
	char *f;
	int len;

	f = strdup(file);
	len = strlen(f);

	f[len-3] = 't';
	fprintf(stderr, "Trying file \'%s\'\n", f);
	if ((t00_fp = fopen(f, "rb"))) {
	    //t00_size(t00_fp);
		fprintf(stderr, "ok\n");
		return 1;
	}

	f[len-3] = 'T';
	fprintf(stderr, "Trying file \'%s\'\n", f);
	if ((t00_fp = fopen(f, "rb"))) {
		fprintf(stderr, "ok\n");
		return 1;
	}

	t00_fp = NULL;

	return 0;
}


char *t00_getrecord(uint32_t record)
{
	uint16_t rec_len;
	uint16_t idx;
	int ret;

	if (t00_fp == NULL) {
		return NULL;
	}

	if (record == 0x00) {
		return NULL;
	}

#ifdef DEBUG
	fprintf(stderr, "record = 0x%x\n", record);
#endif

	fseek(t00_fp, record, SEEK_SET);
	ret = fread(&rec_len, sizeof(uint16_t), 1, t00_fp);
	if (ret != 1) {
		err(1, "zle");
	}

#ifdef DEBUG
	fprintf(stderr, "record = 0x%x, rec_len = %d\n", record, rec_len);
#endif

//	if (rec_len > 10000) {
//		fprintf(stderr, "Zly zaznam %d %x\n", rec_len, rec_len);
//		return NULL;
//	}

	char *rec = malloc(rec_len + 1);
	if (rec == NULL) {
		err(1, "Error allocating memory\n");
	}
	memset(rec, 0, rec_len + 1);

	idx = 0;
	while (rec_len > 0) {
		uint16_t len = DATA_SIZE;

		if ((idx == 0) && (rec_len > DATA_SIZE)) {
			len = DATA_SIZE-4;
		}

		if (idx != 0) {
			if (rec_len > BLOCK_SIZE) {
				len = BLOCK_SIZE-4;
			} else {
				len = BLOCK_SIZE;
			}
		}

		if (len > rec_len) {
			len = rec_len;
		}

#ifdef DEBUG
		fprintf(stderr, "%d %d %d \n", idx, rec_len, len);
#endif
		ret = fread(rec+idx, 1, len, t00_fp);
		if (ret != len) {
#ifdef DEBUG
			size_t filesize = ftell(t00_fp);
			fprintf(stderr, "t00 offset = %x\n", filesize);
#endif
			err(1, "error reading file %d %d\n", ret, len);
		}

		rec_len -= len;
		idx += len;

		// get next record address
		ret = fread(&record, sizeof(uint32_t), 1, t00_fp);
		if (ret != 1) {
			err(1, "error reading address of next block\n");
		}

#ifdef DEBUG
		fprintf(stderr, "next block addr = %08x\n", record);
#endif
		if (record == 0) {
			break;
		}

		fseek(t00_fp, record, SEEK_SET);
	}

	return rec;
}

