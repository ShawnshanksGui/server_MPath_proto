#include "../include/video_reader.h"
#include "system_params.h"

class Path_Selector{
public:
	Channel_Info channel_info[NUM_PATH];
	
	Path_Selector();
	~Path_Selector();

	void set_channel_param();

	void select_Path(Channnel_Info ch_i[NUM_PATH], 
		         Video_Reader &video_reader);

private:

};