#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE *fp = NULL;
	char *buf = (char *)malloc(sizeof(char)*10);
	if((fp = fopen("file_test.txt", "w")) == NULL)
	{
		perror("fopen failed\n");
	}
//	fwrite("hello,world\n", sizeof(char), 12, stdout);
	fwrite("hello,world", sizeof(char), 11, fp);
//	fwrite("hello,world", sizeof(char), 11, fp);
	fp = fopen("file_test.txt", "r");

	fread(buf, sizeof(char), 11, fp);
	printf("%s\n", buf);
//	printf("\n");
//	fwrite(buf, sizeof(char), strlen(buf), stdout);
//	printf("\n");

	return 0;
}
