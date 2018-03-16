#ifndef _PATH_SELECTOR_H_
#define _PATH_SELECTOR_H_

#include "video_reader.h"
#include "system_params.h"


#define WIFI 1
#define CELLULAR_NETWORK 0

class Path_Selector{
public:
	Channel_Inf channel_inf[NUM_PATH];
	
	Path_Selector();
	~Path_Selector();

//	void set_channel_param();

	void select_Path(Channel_Inf ch_i[NUM_PATH], 
		         Video_Reader &video_reader);

private:

};

#endif