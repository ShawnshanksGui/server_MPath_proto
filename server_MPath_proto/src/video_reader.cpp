#include "chrono"
#include "string"

#include "../include/common.h"
#include "../include/my_Hevcdec.h"
#include "../include/video_reader.h"
#include "../include/system.params.h"


Video_Reader::Video_Reader() {
	gop_num      = GOP_NUM;
	gopFrame_num = FRAME_GOP;
	region_num   = REGION_NUM;

	for(int i = 0; i < REGION_NUM; i++) {
		for(int k = 0; k < (FRAME_GOP+10)*GOP_NUM; k++) {
			nalu[i][k]._size = 0;
			nalu[i][k]._addr = 0;
		}
	}
}


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180204
//Description: the thread function which simulates data generating procedure  
//ParameterS:  SYMB_SIZE is equal to encoding symbol size 
//==========================================================================
void Video_Reader::video_reader_td_func(Data_Manager &data_manager,
										int id_VSegment, int id_region) {
	int flag_video = 0;

    std::ifstream File;
    std::string inString;
	
	for(int i = 0, k = 0; i < REGION_NUM; i++) {
		std::string inputVideo_Path;
		inputVideo_Path = "video_????" + std::to_string(bitrate_decs[]i) +
		             "_" + std::to_string(id_VSegment) + ".265";
//==========================================================================
   		File.open(inputVideo_Path, std::ios::in);
   		inString = slurp(File);

    	hevc_probe(inString, i);
    	for(int k = 0; nalu[i][k++].size > 0; k++) {
    		partition_nalu();
    	}
    		
    	}          
//==========================================================================	
}


void Video_Reader::partition_nalu() {
	
}


/*
	int id_path  = 0;
	int _count   = 0;
	int len_read = 0;

	//original data block size.
	int block_size = 200;

	FILE *fp;
	Fopen_for_read(&fp, "input_video.mp4");

	auto startTime = std::chrono::high_resolution_clock::now();

	//set thread core affinity and bind the current thread to core 1 
	affinity_set(DATA_GEN_CORE);
//
	while(1) {
		//real memory allocation function for data generated 
		VData_Type *elem_mem_alloc = MALLOC(char, SYMB_SIZE*block_size);
		len_read = Fread(elem_mem_alloc, SYMB_SIZE*block_size, fp);
		if(END_FILE == len_read) break;

		//until pushed and saved successfully
		while(SUCS_PUSH != data_manager.data_save(elem_mem_alloc, id_path));

		//record the eclpsing time for TEST_SECONDS
		auto endTime  = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds> 
                        (endTime - startTime).count();

		if(duration > TEST_SECONDS*1000000) break;
	}
}
*/		
//==========================================================================