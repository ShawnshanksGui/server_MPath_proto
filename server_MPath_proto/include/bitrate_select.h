#include "../include/video_reader.h"

#define  LOWEST 0
#define  MEDIAN 1
#define HIGHEST 2

class Bitrate_Selector {
public:
	int S_FEC;
	int K_FEC;
	
	Bitrate_Select() {}
	~Bitrate_Select() {}

	void setBitrate(Tile_Num tile_num, Channel_Inf chan_inf, 
					Video_Reader &video_reader); 

private:

};