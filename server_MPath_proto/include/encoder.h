#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "data_manager.h"

class Encoder {
public:
	Encoder() {}
	~Encoder() {}

	void encoder_init();
	void *encode(char *data_src, int S, int K, int M);
//	void encoder_td_func(Data_Manager &data_manager);

//	void partition_nalu(std::string &inString,
//		  				Data_Manager &data_manager);
private:

};
#endif