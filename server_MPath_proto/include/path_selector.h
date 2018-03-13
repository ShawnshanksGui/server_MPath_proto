#ifndef _PATH_SELECTOR_H_
#define _PATH_SELECTOR_H_

#include "video_reader.h"
#include "system_params.h"


#define WIFI 1
#define CELLULAR_NETWORK 0

class Path_Selector{
public:
	Channel_Info channel_info[NUM_PATH];
	
	Path_Selector();
	~Path_Selector();

//	void set_channel_param();

	void select_Path(Channnel_Info ch_i[NUM_PATH], 
		         Video_Reader &video_reader);

private:

};

#endif