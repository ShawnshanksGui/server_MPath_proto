#include <string>
#include <memory>


#include "../include/data_manager.h"
#include "../include/encoder.h"

extern "C"
{
	#include "../include/rs_fft.h"
}

void Encoder::encoder_init()  {
	init();
	init_dec();	
}

void *Encoder::encode(char *data_src, int S, int K) {
	encode(data_src, S, K);
}	
/*
void Encoder::encoder_td_func(Data_Manager &data_manager) {

//	for(int i = 0; ; ;)
}
*/