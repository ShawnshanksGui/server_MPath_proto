#ifndef _FEC_PARAM_ADJUSTOR_H_
#define _FEC_PARAM_ADJUSTOR_H_

#include "common.h"
#include "video_reader.h"
#include "bitrate_select.h"
#include "system_params.h"

#define LEVEL_NUM 7
//actually step_rate = 1/6, for coding conveniently
#define STEP_RATE  FRAME_GOP/6

typedef unsigned char uchar;

class FEC_Param_Adjuster{
public:
	int avail_K[LEVEL_NUM];

	FEC_Param_Adjuster();
	~FEC_Param_Adjuster() {}

	void setFEC_params(Channel_Inf chan_inf[NUM_PATH],
				  Bitrate_Selector &bitrate_selector,
		          Video_Reader &video_reader);
private:

};

#endif