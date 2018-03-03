#include "../include/data_manager.h"

class Video_Reader {
public:
	Video_Reader();
	~Video_Reader() {}

	void video_reader_thread_func(Data_Manager &data_manager);
	friend class Bitrate_Select;
	friend class Path_Decs;
private:
	int  frame_num; 
	int region_num;
 

//  The path decision about each region(FOV, adjacent, outsise) 
//	for every frame in a video segment.  
	int path_decs[frame_num][region_num];

//  THe bitrate decision about each region of a video segment
	int bitrate_decs[region_num];
};
