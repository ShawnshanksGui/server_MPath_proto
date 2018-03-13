#ifndef _FEC_PARAM_ADJUSTOR_H_
#define _FEC_PARAM_ADJUSTOR_H_

#include "common.h"
#include "video_reader.h"
#include "bitrate_select.h"

typedef unsigned char uchar;

class FEC_Param_Adjuster{
public:
	uchar avail_K[6];

	FEC_Param_Adjuster();
	~FEC_Param_Adjuster() {}

	void setFEC_params(Channel_Inf chan_inf[NUM_PATH],
				  Bitrate_Selector &bitrate_selector,
		          Video_Reader &video_reader);
private:

};

#endif