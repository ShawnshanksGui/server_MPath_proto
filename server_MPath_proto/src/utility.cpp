#include  "common.h"

void Usage(int argc, int _cnt, char *prog_name) {
    if(argc != _cnt) {
        printf("-help usage: %s client_addr client_port server_addr \
            server_port open_filename", prog_name);
    }
}

void Fopen_for_read(FILE **fp, char *param) {
    if((*fp = fopen(param, "rt+")) == NULL) {
        perror("Open file failed!\n");
        exit(0);
    }
}

void Fopen_for_write(FILE **fp, char *param) {
    if((*fp = fopen(param, "wt+")) == NULL) {
        perror("Open file failed!\n");
        exit(0);
    }
}

int Fread(char *dest, int len, FILE *fp_src) {
    int num;

    if((num = fread(dest, sizeof(char), len, fp_src)) < 0) {
        perror("read from file failed!!!\n");
        exit(0);
    }
    else {
        return num;
    }
}

int Fwrite(char *src, int len, FILE *fp_dest) {
    int num;

    if((num = fwrite(src, sizeof(char), len, fp_dest)) < 0) {
        perror("write into file failed!!!\n");
        exit(0);
    }
    else {
        return num;
    }
}
/*
void *Malloc(size_t size) {
	void *p = NULL;
	if((p = malloc(size)) == NULL) {
		perror("Malloc failed");
		exit(0);
	}
	return p;
}
*/
