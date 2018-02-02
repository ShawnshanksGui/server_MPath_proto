#ifndef _RS_FFT_H
#define _RS_FFT_H

void init();
void init_dec();

void *encode_FFT_RS(char    * data_src, struct Para_Encd para_encd); 
void *decode_FFT_RS(struct Data_Remain, struct Para_Decd para_decd); 

#endif
