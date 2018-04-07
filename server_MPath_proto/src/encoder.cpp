#include <string>
#include <memory>

#include "../include/data_manager.h"
#include "../include/encoder.h"

//specidies whether FFT_RS or RS
#define ENABLE_FFT_RS

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


#elif
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
