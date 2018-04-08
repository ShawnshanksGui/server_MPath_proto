#include <string>
#include <memory>

#include "../include/system_params.h"
#include "../include/data_manager.h"
#include "../include/encoder.h"

#ifdef ENABLE_FFT_RS
extern "C"
{
	#include "../include/rs_fft.h"
}
void Encoder::encoder_init()  {
	init();
	init_dec();	
}

void *Encoder::encode(char *data_src, int S, int K, int M) {
	return fft_encode(data_src, S, K);
}	


#else
extern "C"
{
	#include "../include/rs.h"
}

void Encoder::encoder_init()  {

}

void *Encoder::encode(char *data_src, int S, int K, int M) {
	return rs_encode(data_src, S, K, M);
}	
#endif

/*
void Encoder::encoder_td_func(Data_Manager &data_manager) {

//	for(int i = 0; ; ;)
}
*/
