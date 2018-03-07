#include "../include/video_reader.h"
#include "system.params.h"
#include <chrono>

Video_Reader::Video_Reader() {
	frame_num = FRAME_GOP;
	region_num = REGION_NUM;
}

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180204
//Description: the thread function which simulates data generating procedure  
//Parameter:   SYMB_SIZE is equal to encoding symbol size 
//==========================================================================
void Video_Reader::video_reader_td_func(Data_Manager &data_manager) {
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
                        (endTime - startTime ).count();

		if(duration > TEST_SECONDS*1000000) break;
	}	
}
//==========================================================================