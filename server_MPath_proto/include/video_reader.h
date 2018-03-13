#ifndef _VIDEO_READER_H
#define _VIDEO_READER_H

#include "system_params.h"
#include "data_manager.h"


//the intra pridicted frame
#define I_FRAME 1
//the forward predicted frame
#define P_FRAME 0


struct Nalu_Elem{
	bool frameType;
//the start address for each NALU. 
	int _addr;
//the size for each NALU.
	int _size;
};


class Video_Reader {
public:
	int S_FEC[REGION_NUM][FRAME_GOP];
	int K_FEC[REGION_NUM][FRAME_GOP];	

	struct Nalu_Elem nalu[REGION_NUM][FRAME_GOP*GOP_NUM];

	Video_Reader();
	~Video_Reader() {}

	friend class Bitrate_Selector;
	friend class Path_Decs;

private:
	int region_num;
	int gop_num;
	int gopFrame_num; 
	
//  The path decision about each region(FOV, adjacent, outsise) 
//	for every frame in a video segment.  
	int path_decs[REGION_NUM][FRAME_GOP*GOP_NUM];

//  THe bitrate decision about each region of a video segment
//the first is the bitrate of FOV, 
	int bitrate_decs[REGION_NUM];

	void video_reader_td_func(Data_Manager &data_manager, int id_VSegment);
	void partition_nalu(int id_region, VData_Type *p_str, 
						Data_Manager &data_manager);
	void assign_attribute(struct Elem_Data *elem_data, int path,
						  int s_fec, int k_fec, int id_nalu,
						  int _addr, VData_Type *p_str,
						  Data_Manager &data_manager);
};

#endif