#ifndef _RS_FFT_H
#define _RS_FFT_H

typedef unsigned char Indicator_Type;

struct Data_Remain{
//	_Bool erasure[256]; //erasure[N]
	Indicator_Type erasure[256];
	char data[256][10]; //data[N][S]
};

struct Param_Encd {
	int S;  //symbol size
	int K;  //original data block size
};

struct Param_Decd {
	int S;	//symbol size
	int K;  //original data block size
};

void init();
void init_dec();

void *fft_encode(char *data_src, int S, int K);
void *fft_decode(char *data_recv, char *erasure, int S, int K);
void *encode_FFT_RS(char    * data_src, struct Param_Encd param_encd); 
void *decode_FFT_RS(struct Data_Remain, struct Param_Decd param_decd); 

#endif	
