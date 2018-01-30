#include  "common.h"


void usage(char *command)
{
	printf("usage:%s sender_add sender_port open_filename", command);
}

void Usage(int argc, int _cnt, char *prog_name){
	if(argc != _cnt) {
		usage(prog_name);
	}
}

void Fopen(FILE **fp, char *param)
{
	if((*fp = fopen(param, "w")) == NULL)
	{
		perror("Open file failed\n");
		exit(0);
	}
}