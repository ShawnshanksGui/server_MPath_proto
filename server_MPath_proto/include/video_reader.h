#ifndef _VIDEO_READER_H
#define _VIDEO_READER_H

#include <string>
#include <memory>

#include "system_params.h"
#include "data_manager.h"
//#include "codeStreaming_parser.h"

#define VAL_FOR_INIT 2000

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

class A {
public:
	int num;
};


class Video_Reader {
public:
	int S_FEC[REGION_NUM][FRAME_GOP];
	int K_FEC[REGION_NUM][FRAME_GOP];
	int M_FEC[REGION_NUM][FRAME_GOP];	

	struct Nalu_Elem nalu[REGION_NUM][FRAME_GOP*GOP_NUM];

	int path_decs[REGION_NUM][FRAME_GOP*GOP_NUM];

//  THe bitrate decision about each region of a video segment
//the first is the bitrate of FOV, 
	int bitrate_decs[REGION_NUM];

	Video_Reader();
	~Video_Reader() {}

//for test
	void video_reader_func(Data_Manager &data_manager, int id_VSegment);
//	void video_reader_func(A &a, int id_VSegment) {}

	void video_reader_td_func(Data_Manager &data_manager, int id_VSegment);
	void reader_init();

	friend class Data_Manager;
	friend class Bitrate_Selector;
	friend class Path_Decs;
	friend class Path_Selector;

	friend std::string slurp(std::ifstream &File);
	friend void partition_nalu(int id_region, VData_Type *cstr, Data_Manager &data_manager);

private:
	int region_num;
	int gop_num;
	int gopFrame_num; 
	
//	void setVideoReader(Data_Manager &&_data_manger);

//  The path decision about each region(FOV, adjacent, outsise) 
//	for every frame in a video segment.  

	void partition_nalu(int id_region, VData_Type *p_str, int id_seg, 
						Data_Manager &data_manager);
	void assign_attributes(int cnt_blk, shared_ptr<struct Elem_Data> elem_data,
						   int path, int s_fec, int k_fec, int m_fec, int id_nalu,
						   int _addr, VData_Type *p_str, Data_Manager &data_manager);
};

#endif