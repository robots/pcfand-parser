#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "t00.h"
#include "linklist.h"

unsigned int start_offset = 6;

int main(int argc, char **argv) {
	FILE *in_fp;
	FILE *fp_out;
	FILE *fp_null;
	FILE *struct_fp;

	char inputfile[100];
	char outputfile[100];
	char sqlquery[1000];
	char header[10];

	long filesize;
	uint32_t id = 0;
	uint32_t rec = 0;
	uint32_t del = 0;
	int32_t expected_rec;

	if (argc < 2) {
		fprintf(stderr, "Missing argument \n %s script_file", argv[0] );
		exit(1);
	}

/*priprav  sa */
	struct_fp = fopen(argv[1], "rb");
	if (struct_fp == NULL) {
		fprintf(stderr, "Input file '%s' error \n", argv[1]);
		exit(1);
	}

	fgets(inputfile, 100, struct_fp);
	fgets(outputfile, 100, struct_fp);
	fgets(sqlquery, 1000, struct_fp);

	inputfile[strlen(inputfile)-1] = '\0';
	outputfile[strlen(outputfile)-1] = '\0';
	sqlquery[strlen(sqlquery)-1] = '\0';

	while (ll_fill(struct_fp));

/* pozor */
	in_fp = fopen(inputfile, "rb");

	t00_init(inputfile);

	if (in_fp == NULL) {
		fprintf(stderr, "Input file '%s' error\n", inputfile);
		exit(1);
	}
	fp_out = fopen(outputfile, "w");

	fp_null = fopen("/dev/null", "w");

	fseek(in_fp, 0, SEEK_END);
	filesize = ftell(in_fp);
	//fseek(in_fp, start_offset, SEEK_SET);
	fseek(in_fp, 0, SEEK_SET);

	fread(&expected_rec, sizeof(int32_t), 1, in_fp);
	// TODO: ? ? ? ? ?
	fread(header, sizeof(char), 2, in_fp);

	//ll_id();

	fprintf(stderr, "%s -> %s \n", inputfile, outputfile);
	fprintf(fp_out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Root>\n");
/* start */
	while (filesize - ftell(in_fp)>0)
	{
		char b;
		FILE *outputfile = fp_out;

		if (expected_rec < 0) {
			fread(&b, sizeof(char), 1, in_fp);

			if (b != 0x00) {
				if (b & 0x01) {
//					fprintf(stderr, "deleted record, skipping\n");
					del++;
					outputfile = fp_null;
				}

				if (b & 0xFE) {
					fprintf(stderr, "something feels different record %d, b=0x%02x\n", rec, b);
					exit(-1);
				}
			}
		} else {
			b = 0;
		}

		fprintf(outputfile, "<%s id=\"%d\" ", sqlquery, id);
		ll_walkthrough(in_fp, outputfile);
		fprintf(outputfile, "/>\n");

		if ((b & 0x01) != 0x01) {
			// not deleted ... inc id
			id++;
		}
		rec++;
	}
	fprintf(fp_out, "\n</Root>");
	fprintf(stderr, " ... %d records processed, %d correct, %d deleted, expected %d\n", rec, id, del, expected_rec);


	ll_destroy();
	fclose(fp_out);
	fclose(fp_null);
	fclose(in_fp);
	fclose(struct_fp);

	return 0;
}
