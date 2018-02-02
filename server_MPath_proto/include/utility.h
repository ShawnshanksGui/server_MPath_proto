void Usage(int argc, int _cnt, char *prog_name);

void Fopen_for_read(FILE **fp, char *param);
void Fopen_for_write(FILE **fp, char *param);

int Fread(char *dest, int len, FILE *fp_src);
int Fwrite(char *src, int len, FILE *fp_dest);

//void *Malloc(size_t size);
