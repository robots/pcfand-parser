
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include "types.h"
#include "t00.h"

#define MAX_SIZE 200

int parse_N(FILE *fp, struct pData *p, FILE *fp_out) {
	int ret;
	int readnum = *((int *)(p->data));
	int num = readnum;
	int i = 0;
	int tmp;
	char buf[MAX_SIZE];

	if (readnum % 2 == 1) 
		readnum ++;
	readnum /= 2;

	ret = fread(buf, sizeof(char), readnum, fp);
	if (ret != readnum) return 2;

	if (p->db_col != NULL) {
		fprintf(fp_out,"%s=\"", p->db_col);

		for (i = 0; i<num; i++) {
			tmp = buf[i/2] >> 4;
			if(i % 2 == 0) {
				tmp = buf[i/2] >> 4;
			} else {
				tmp = buf[i/2];
			}
			tmp = tmp & 0x0f;
			tmp += '0';
			fprintf(fp_out, "%c",tmp);
		}
		fprintf(fp_out, "\"");
		return 1;
	}

	return 0;
}

int parse_AZ(FILE *fp, struct pData *p, FILE *fp_out, int x) {
	int ret;
	int i;
	int num = *((int *)(p->data));
	char buf[MAX_SIZE];
	memset(buf, 0, MAX_SIZE);
	
	ret = fread(buf, sizeof(char), num, fp);
	
	if (ret != num) return 2;

	if (x) {
		for(i = 0; i < strlen(buf); i++) {
			buf[i] = buf[i] ^ 0xAA;
		}
	}

	trim_space(buf);
	if (p->db_col != NULL) {
		fprintf(fp_out, "%s=\"", p->db_col);
		for(i = 0; i < strlen(buf); i++) {
			if (buf[i] == '\"') {
			    fprintf(fp_out, "&quot;");
			} else if (buf[i] == '\'') {
			    fprintf(fp_out, "&apos;");
			} else if (buf[i] == '&') {
			    fprintf(fp_out, "&amp;");
			} else if (buf[i] == '>') {
			    fprintf(fp_out, "&gt;");
			} else if (buf[i] == '<') {
			    fprintf(fp_out, "&lt;");
			} else if (!isprint(buf[i])) {
			} else {
				fprintf(fp_out, "%c", buf[i]);
			}
		}
		fprintf(fp_out, "\"");
		return 1;
	}
	return 0;
}

int parse_A(FILE *fp, struct pData *p, FILE *fp_out) {
	return parse_AZ(fp, p, fp_out, 0);
}

int parse_Z(FILE *fp, struct pData *p, FILE *fp_out) {
	return parse_AZ(fp, p, fp_out, 1);
}

int parse_F(FILE *fp, struct pData *p, FILE *fp_out) {
	int ret;
	char outstr[40];
	struct tFloat *input = (struct tFloat*)(p->data);
	int readnum = 0;
	unsigned long long full;
	unsigned long long out;
	unsigned char ch;
	char sign = 0;
	int i;

	memset (&out, 0, sizeof(out));

	if (input->total < 3) {
		readnum = 1;
	} else if ((input->total >=3) && (input->total < 5)) {
		readnum = 2;
	} else if ((input->total >= 5) && (input->total < 7)) {
		readnum = 3;
	} else if ((input->total >= 7) && (input->total < 10)) {
		readnum = 4;
	} else if ((input->total >= 10) && (input->total < 12)) {
		readnum = 5;
	} else if (input->total >= 12) {
		readnum = 6;
	}

	sign = 0;
	out = 0;
	full = 0;
	for(i=0; i<readnum; i++) {
		full = full << 8;
		out = out << 8;

		ret = fread(&ch, sizeof(unsigned char), 1, fp);
		full = full | 0xff;
		out = out | ch;
		if (ret != 1) return 2;

		if ((i == 0) && (ch & 0x80)) {
			sign = 1;
		}
	}

	if (sign) {
		out = full - out + 1;
	}
	
//	sprintf(outstr, "%%s=\"%%c%%.%df\"", input->prec);
	if (input->prec == 0) {
		sprintf(outstr, "%%s=\"%%s%%lld\"");
	} else {
		sprintf(outstr, "%%s=\"%%s%%lld.%%0%dlld\"", input->prec);
	}
	if (p->db_col != NULL) {
		unsigned long long div = 1;
		unsigned long long c, d;
		
		for (i = 0; i < input->prec; i++) div *= 10;

		c = out / div;
		d = out % div;

		if (input->prec == 0) {
			fprintf(fp_out, outstr,  p->db_col, sign?"-":"", c);
		} else {
			fprintf(fp_out, outstr,  p->db_col, sign?"-":"", c, d);
		}
		return 1;
	}
	return 0;
}

int parse_D(FILE *fp, struct pData *p, FILE *fp_out) {
	int ret;
	char date[400];
	unsigned char in[6];
	unsigned long out;

	memset (&out, 0, sizeof(out));
	ret = fread(in, sizeof(unsigned char), 6, fp);

	if(ret != 6)
		return 2;

	/* pure magick - fand datum na unix timestamp */
	out = (in[3] >> 4) | (in[4] << 4) | (in[5] << 12);
	out -= 194859+16;
	out *= 24*3600;

	strftime(date, 40, "%d/%m/%y", localtime((const time_t *)&out));

	if (p->db_col != NULL) {
		fprintf(fp_out, "%s=\"%s\"", p->db_col, date);
		return 1;
	}

/*
	out = (in[3] >> 4) + (in[4] * 16) + (in[5] * 4096);
	out -= 194859;
	out *= 24*3600;

	strftime(date, 40, "%d/%m/%y", localtime((const time_t *)&out));
	if (p->db_col != NULL) {
		fprintf(fp_out, "%s='%s'", p->db_col, date);
		return 1;
	}
	*/
	return 0;
}

int parse_B(FILE *fp, struct pData *p, FILE *fp_out) {
	int ret;
	char in[6];

	ret = fread(in, sizeof(unsigned char), 1, fp);

	if (ret != 1)
		return 2;

	if (in[0] > 1) {
		fprintf(stderr," bad boolean \n");
		return 2;
	}

	//printf("%x %x\n", (int)in[0] & 0xff, (int)in[1]  & 0xff);
	if (p->db_col != NULL) {
		fprintf(fp_out, "%s=\"%x\"", p->db_col, (unsigned int)in[0] & 0xff);
		return 1;
	}
	return 0;
}

int parse_T(FILE *fp, struct pData *p, FILE *fp_out) {
	uint32_t record;
	int i;
	int ret;

	ret = fread(&record, sizeof(uint32_t), 1, fp);
	if(ret != 1)
		return 2;

	if (p->db_col != NULL) {
		char *rec = t00_getrecord(record);

		fprintf(fp_out, "%s=\"", p->db_col);

		if (rec != NULL) {
			for(i = 0; i < strlen(rec); i++) {
				if (rec[i] == '\"') {
						fprintf(fp_out, "&quot;");
				} else if (rec[i] == '\'') {
						fprintf(fp_out, "&apos;");
				} else if (rec[i] == '&') {
						fprintf(fp_out, "&amp;");
				} else if (rec[i] == '>') {
						fprintf(fp_out, "&gt;");
				} else if (rec[i] == '<') {
						fprintf(fp_out, "&lt;");
#if 1
				} else if (rec[i] == 0x0d) {
						fprintf(fp_out, "&#xD;");
				} else if (rec[i] == 0x0a) {
						fprintf(fp_out, "&#xA;");
				} else if (rec[i] == '\t') {
						fprintf(fp_out, "&#x9;");
#endif
				} else if (!isprint(rec[i])) {
				} else {
					fprintf(fp_out, "%c", rec[i]);
				}
			}
			//fprintf(fp_out, "%s", rec);
			free(rec);
		}
		fprintf(fp_out, "\"");
		return 1;
	}

	return 0;
}

int parse_H(FILE *fp, struct pData *p, FILE *fp_out) {
	int ret;
	int i;
	int num = *((int *)(p->data));
	char buf[MAX_SIZE];
	memset(buf, 0, MAX_SIZE);
	
	ret = fread(buf, sizeof(char), num, fp);
	
	if (ret != num) return 2;

	if (p->db_col != NULL) {
		fprintf(fp_out, "%s=\"", p->db_col);
		for(i = 0; i < num; i++) {
			fprintf(fp_out, "0x%x ",buf[i] & 0xff);
		}
		fprintf(fp_out, "\"");
		return 1;
	}
	return 0;
}

void trim_space(char * input) {
	char *tmp = input;
	tmp += strlen(tmp)-1;
	while(*tmp == ' ') {
		*tmp = '\0';
		tmp--;
	}
}

