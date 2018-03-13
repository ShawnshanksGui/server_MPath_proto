#ifndef _BITRATE_SELECT_H
#define _BITRATE_SELECT_H

#include "video_reader.h"
#include "system_params.h"

#define  LOWEST 0
#define  MEDIAN 1
#define HIGHEST 2

class Bitrate_Selector {
public:
	int S_FEC;
	int K_FEC;
	
	char bitrate[BITRATE_TYPE_NUM];

	Bitrate_Selector() {}
	Bitrate_Selector(int _bitrate[BITRATE_TYPE_NUM]);

	~Bitrate_Selector() {}

	void setBitrate(int tile_num[], Channel_Inf chan_inf, 
					Video_Reader &video_reader); 

private:

};

#endif
