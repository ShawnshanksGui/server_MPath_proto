#include "../include/data_manager.h"


class Video_Reader {
public:
	int S_FEC[REGION_NUM][FRAME_GOP];
	int K_FEC[REGION_NUM][FRAME_GOP];	

	Video_Reader();
	~Video_Reader() {}

	void video_reader_td_func(Data_Manager &data_manager, int id_VSegment);

	friend class Bitrate_Selector;
	friend class Path_Decs;

private:
	int region_num;
	int gop_num;
	int gopFrame_num; 
	

//  The path decision about each region(FOV, adjacent, outsise) 
//	for every frame in a video segment.  
	int path_decs[frame_num][region_num];

//  THe bitrate decision about each region of a video segment
//the first is the bitrate of FOV, 
	int bitrate_decs[region_num];
};