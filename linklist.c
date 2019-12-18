
#include <string.h>
#include "linklist.h"
#include "types.h"

struct tNode *First=NULL;
struct tNode *Last=NULL;

int parts = 0;

void ll_id(void)
{
	struct tNode *tmp = First;
	while(tmp) {
		printf("%s",tmp->Name);
		if (tmp->Data->db_col)
			printf(" -> %s ", tmp->Data->db_col);
/*		switch(tmp->ID) {
			case TYPE_A: 
				printf("%d",*(int *)(tmp->Data));
				break;
			default:
				break;
		}*/
		if(tmp->Next != NULL) {
			printf(";\n");
		}
		tmp = tmp->Next;
	}

	printf("\n");
}

void ll_walkthrough(FILE* fp, FILE *fp_out)
{
	int ret = 0;
	int cnt = 1;
	struct tNode *tmp = First;
	while(tmp) {
		ret = tmp->Command(fp, tmp->Data, fp_out);
		
		if (ret == 2) {
			fprintf(stderr,"error");
			exit(1);
		}

		if(tmp->Next == NULL) {
//			fprintf(fp_out, "");
		} else {
			if ((ret == 1) && cnt < parts) {
				cnt ++;
				fprintf(fp_out, " ");
			}
		}
		tmp = tmp->Next;
	}
}

void ll_destroy(void)
{
	struct tNode *tmp;
	struct tNode *in=First;
	
	while (in) {
		tmp = in->Next;
		if (in->Data->db_col)
			free (in->Data->db_col);
		if (in->Data->data)
			free(in->Data->data);
		free(in->Data);
		free(in->Name);
		free(in);
		in = tmp;
	}
}

int ll_fill(FILE* fpin)
{
	struct tNode *tmp;
	char *type;

	tmp = malloc(sizeof(struct tNode));
	tmp->Name = get_token(fpin);
	if (tmp->Name == NULL) {
		free(tmp);
		return 0;
	}
	tmp->Next = NULL;
	type = get_token(fpin);

	if(strncmp(type, "N", 1) == 0) {
		//cele cislo
		char *temp_token = get_token(fpin);
		struct pData *p = malloc(sizeof(struct pData));
		int *num = malloc(sizeof(int));
		*num = atoi(temp_token);
		free(temp_token);

		p->data = (void *)num;
		p->db_col = get_token(fpin);
		if (p->db_col != NULL) parts++;

		tmp->ID = TYPE_N;
		tmp->Data = p;
		tmp->Command = parse_N;
	} else if(strncmp(type, "A", 1) == 0) {
		//ascii string
		char *temp_token = get_token(fpin);
		struct pData *p = malloc(sizeof(struct pData));
		int *num = malloc(sizeof(int));
		*num = atoi(temp_token);
		free(temp_token);

		p->data = (void *)num;
		p->db_col = get_token(fpin);
		if (p->db_col != NULL) parts++;

		tmp->ID = TYPE_A;
		tmp->Data = p;
		tmp->Command = parse_A;
	} else if(strncmp(type, "Z", 1) == 0) {
		//ascii string xor-ed
		char *temp_token = get_token(fpin);
		struct pData *p = malloc(sizeof(struct pData));
		int *num = malloc(sizeof(int));
		*num = atoi(temp_token);
		free(temp_token);

		p->data = (void *)num;
		p->db_col = get_token(fpin);
		if (p->db_col != NULL) parts++;

		tmp->ID = TYPE_Z;
		tmp->Data = p;
		tmp->Command = parse_Z;
	} else if(strncmp(type, "F", 1) == 0) {
		//float 
		int x,y;
		char *temp_token = get_token(fpin);
		struct pData *p = malloc(sizeof(struct pData));
		struct tFloat *flot = malloc(sizeof(struct tFloat));
		x = atoi(temp_token);
		free(temp_token);

		temp_token = get_token(fpin);
		y = atoi(temp_token);
		free(temp_token);

		flot->total= x+y;
		flot->prec = y;

		p->data = (void *)flot;
		p->db_col = get_token(fpin);
		if (p->db_col != NULL) parts++;

		tmp->ID = TYPE_F;
		tmp->Data = p;
		tmp->Command = parse_F;
	} else if(strncmp(type, "D", 1) == 0) {
		char *temp_token = get_token(fpin);
		struct pData *p = malloc(sizeof(struct pData));

		p->data = temp_token;
		p->db_col = get_token(fpin);
		if (p->db_col != NULL) parts++;

		tmp->ID = TYPE_D;
		tmp->Data = p;
		tmp->Command = parse_D;
	} else if (strncmp(type, "B", 1) == 0) {
		struct pData *p = malloc(sizeof(struct pData));

		p->data = NULL;
		p->db_col = get_token(fpin);
		if (p->db_col != NULL) parts++;

		tmp->ID = TYPE_B;
		tmp->Data = p;
		tmp->Command = parse_B;

	} else if (strncmp(type, "T", 1) == 0) {
		struct pData *p = malloc(sizeof(struct pData));

		p->data = NULL;
		p->db_col = get_token(fpin);
		if (p->db_col != NULL) parts++;

		tmp->ID = TYPE_T;
		tmp->Data = p;
		tmp->Command = parse_T;

	} else if(strncmp(type, "H", 1) == 0) {
		//HEX string
		char *temp_token = get_token(fpin);
		struct pData *p = malloc(sizeof(struct pData));
		int *num = malloc(sizeof(int));
		*num = atoi(temp_token);
		free(temp_token);

		p->data = (void *)num;
		p->db_col = get_token(fpin);
		if (p->db_col != NULL) parts++;

		tmp->ID = TYPE_H;
		tmp->Data = p;
		tmp->Command = parse_H;
	} else {
//		printf("asi koniac\n");
		return 0;
	}
    
	if(First == NULL) {
		First = Last = tmp;
	} else {
		Last->Next = tmp;
		Last = Last->Next;
	}
	return 1;
}

#define MAX_SIZE 200
char *get_token(FILE* fp)
{
	char tmp[MAX_SIZE];
	char *out, ch;
	int index = 0;
	int in_string = 0;

	memset(tmp, 0, MAX_SIZE);
	fread(&ch, sizeof(char), 1, fp);
	if(ch=='\'') in_string = 1;
	while((
			(ch != ':') &&
			(ch != '.') &&
			(ch != ',') &&
			(ch != ';'))
			||(in_string)) {
		tmp[index]=ch;
		index++;
		fread(&ch, sizeof(char), 1, fp);
		if(ch=='\'') in_string = 0;
	}
	
	if(ch==';') {
		fseek(fp, 1, SEEK_CUR);
	}
	
	if (index == 0) return NULL;

	out = malloc(index+1);
	memset(out, 0, index+1);	
	memcpy(out, tmp, index);
	
	return out;
}
