#include "video_reader.h"
#include "include/bitrate_selector.h"

typedef unsigned char uchar;

class FEC_Param_Adjuster{
public:
	uchar avail_K[6];

	FEC_Param_Adjuster();
	~FEC_Param_Adjuster();

	setFEC_param(Channel_Inf chan_inf[NUM_PATH], 
		         Bitrate_Selector &bitrate_selector,
				 Video_Reader &video_reader);
private:

};