#include "data_manager.h"

extern "C"
{
	#include "../include/rs_fft.h"
}
class Encoder {
public:
	Encoder() {}
	~Encoder() {}

	void encoder_init();
	void *encode(char *data_src, int S, int K);
//	void encoder_td_func(Data_Manager &data_manager);

//	void partition_nalu(std::string &inString,
//		  				Data_Manager &data_manager);
private:

};

