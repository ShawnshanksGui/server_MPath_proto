#include "string"


#include "../include/data_manager.h"

extern "C"
{
	#include "../include/rs_fft.h"
}

class Encoder {
public:
	Encoder() {}
	~Encoder() {}

	void encoder_td_func();

	void partition_nalu(std::string &inString,
		  				Data_Manager &data_manager);
private:

};

void Encoder::encoder_td_func(Data_Manager &data_manager) {

	for(int i = 0; i < )
}


