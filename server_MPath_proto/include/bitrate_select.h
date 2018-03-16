#ifndef _BITRATE_SELECT_H
#define _BITRATE_SELECT_H

#include "common.h"
#include "video_reader.h"
#include "system_params.h"

#define  LOWEST 2
#define  MEDIAN 1
#define HIGHEST 0

class Bitrate_Selector {
public:
	int S_FEC;
	int K_FEC;
	
	char bitrate[BITRATE_TYPE_NUM];

	Bitrate_Selector() {}
	Bitrate_Selector(double _bitrate[BITRATE_TYPE_NUM]);

	~Bitrate_Selector() {}

	void setBitrate(int tile_num[REGION_NUM], 
					Channel_Inf chan_inf[NUM_PATH], 
					Video_Reader &video_reader); 

private:

};

#endif
