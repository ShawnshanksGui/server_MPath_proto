#include "../../include/rs_fft.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

int main() {
	char   data_src[128*10]         = {'\0'};
	char   *data_encd               = NULL;
	char   *data_recovery           = NULL;
	struct Param_Encd param_encd      = {10, 128};
	struct Param_Decd param_decd      = {10, 128};
	struct Data_Remain data_remain;

	init();
	init_dec();	
//encoding 
	char num = 97;
	for(int i = 0; i < 128; i++) {
		memset(&data_src[i*10], num, 10);
		num++;
		if(122 == num) {num = 97;}
	} 
	printf("\n\n");

	data_encd = encode_FFT_RS(data_src, param_encd);
//	printf("%s\n\n", data_encd);

//simulate erasure
	srand(time(NULL));
	for(int i = 0; i < 256; i++) {
		if((rand()%100/100.0) < 0.5) {
			data_remain.erasure[i] = 1;
		}
		else {data_remain.erasure[i] = 0;}
	}
	for(int i = 0; i < 256; i++) {
		if(0 == data_remain.erasure[i]) {
			memcpy(data_remain.data[i], &data_encd[i*param_decd.S], param_decd.S);
		}
	}
//decoding
	data_recovery = decode_FFT_RS(data_remain, param_decd); 

	free(data_encd);
	free(data_recovery);

	return 0;
}