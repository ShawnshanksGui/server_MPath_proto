#include "chrono"
#include "string"
#include "vector"

#include "../include/common.h"
#include "../include/video_reader.h"
#include "../include/data_manager.h"
#include "../include/system_params.h"
#include "../include/path_selector.h"
#include "../include/codeStreaming_parser.h"

//for debugging
#define ENABLE_DEBUG_READER

#ifdef  ENABLE_DEBUG_READER

#include <thread>
#include "../include/utility.h"
#include "../include/bitrate_select.h"
#include "../include/fec_param_adjustor.h"

#endif

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
//Author:      shawnshanks_fei         Date:        20180311
//Description: the thread function which simulates data generating procedure  
//Parameters:  SYMB_SIZE is equal to encoding symbol size 
//==========================================================================
void Video_Reader::video_reader_td_func(Data_Manager &data_manager,
										int id_VSegment) {
	int flag_video = 0;

    std::ifstream File;
    std::string inString;
	

	for(int i = 0; i < REGION_NUM; i++) {
		std::string inputVideo_Path;
		inputVideo_Path = "video_????" + std::to_string(bitrate_decs[i]) +
		             "_" + std::to_string(id_VSegment) + ".265";
//==========================================================================
   		File.open(inputVideo_Path, std::ios::in);
   		inString = slurp(File);
//replicate data to a safe location
   		VData_Type *cstr = new VData_Type[inString.length() + 1];
   		strcpy(cstr, inString.c_str());

   		data_manager.data_vec.push_back(cstr);

    	hevc_parser(inString, i);

    	partition_nalu(i, cstr, data_manager);

    	delete [] cstr;
    } 
//==========================================================================	
}


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180312
//Description: split the video segment into data block with equal length(S*K)  
//Parameters:  result stored into the Data_Mannager.queue<struct Elem_Data*>
//==========================================================================
void Video_Reader::partition_nalu(int id_region, VData_Type *p_str,
								  Data_Manager &data_manager) {
//	const VData_Type *p_str = inString.c_str();
	for(int i = 0; i < FRAME_GOP*GOP_NUM; i++) {
//get the start address for the current nalu(or frame);
		int location = nalu[id_region][i]._addr;

		int s_fec = S_FEC[id_region][i%FRAME_GOP];
		int k_fec = K_FEC[id_region][i%FRAME_GOP];

//remainder number 
		int len_remaining = nalu[id_region][i]._size % (s_fec*k_fec);
//
		int num = nalu[id_region][i]._size / (s_fec*k_fec);

//real start address of current NALU(or frame) in inString data.
		p_str = p_str + nalu[id_region][i]._addr;

//when the length of nalu is divisible(num-1 == k) by 
//the product of s_fec and k_fec
		if(0 == len_remaining) {
//when the current nalu is longer than block size(s_fec*k_fec).
			for(int k = 0; k < num; i++){
				struct Elem_Data *elem_data = MALLOC(struct Elem_Data, 1);
				if(0 == i ) {
					if(0 == k)
						elem_data->type_location = FRONT_FOR_FIRST_NALU;
					else if (num-1 == k) //not divisible
						elem_data->type_location = END_FOR_FIRST_NALU;
					else {elem_data->type_location = MID_FOR_FIRST_NALU;}
				}
				else if(FRAME_GOP*GOP_NUM-1 == i) {
					if(0 == k)
						elem_data->type_location = FRONT_FOR_END_NALU;
					else if (num-1 == k) //not divisible
						elem_data->type_location = MID_FOR_END_NALU;
					else {elem_data->type_location = END_FOR_END_NALU;}					
				}
				else {
					if(0 == k)
						elem_data->type_location = FRONT_FOR_MID_NALU;
					else if (num-1 == k) //not divisible 
						elem_data->type_location = MID_FOR_MID_NALU;
					else {elem_data->type_location = END_FOR_MID_NALU;}
				}
				elem_data->size = s_fec*k_fec;

				assign_attribute(elem_data, path_decs[id_region][i], s_fec,
				 k_fec, i, location, p_str, data_manager);
				location += s_fec*k_fec;
			}
		}
//when the length of nalu is not divisible, (num == k)
		else {
			for(int k = 0; k <= num; k++) {
				struct Elem_Data *elem_data = MALLOC(struct Elem_Data, 1);
				if(0 == i ) {
					if(0 == k)
						elem_data->type_location = FRONT_FOR_FIRST_NALU;

					else if (num == k) // divisible
						elem_data->type_location = END_FOR_FIRST_NALU;
					else {elem_data->type_location = MID_FOR_FIRST_NALU;}
				}
				else if(FRAME_GOP*GOP_NUM-1 == i) {
					if(0 == k)
						elem_data->type_location = FRONT_FOR_END_NALU;
					else if (num == k) // divisible 
						elem_data->type_location = MID_FOR_END_NALU;
					else {elem_data->type_location = END_FOR_END_NALU;}					
				}
				else {
					if(0 == k)
						elem_data->type_location = FRONT_FOR_MID_NALU;
					else if (num == k)  // divisible
						elem_data->type_location = MID_FOR_MID_NALU;
					else {elem_data->type_location = END_FOR_MID_NALU;}
				}
				if(k == num) {
					elem_data->size = nalu[id_region][i]._size-location;
				}
				else {elem_data->size = s_fec*k_fec;}

				assign_attribute(elem_data, path_decs[id_region][i], s_fec,
				 				 k_fec, i, location, p_str, data_manager);
				location += s_fec*k_fec;
			}
		}
	}
}
//==========================================================================


void Video_Reader::assign_attribute(struct Elem_Data *elem_data,int path, 
									int s_fec, int k_fec, int id_nalu,
									int _addr, VData_Type *p_str,
									Data_Manager &data_manager) {
	if(0 == (id_nalu % FRAME_GOP)) {
		elem_data->type_nalu = I_FRAME;
	}
	else {elem_data->type_nalu = P_FRAME;}

	elem_data->data    = p_str + _addr;
	elem_data->id_path = path;
	elem_data->id_nalu = id_nalu;
	elem_data->S_FEC   = s_fec;
	elem_data->K_FEC   = k_fec;

	data_manager.data_save(elem_data, elem_data->id_path);
}




#ifdef ENABLE_DEBUG_READER

//two channels' realtime infomation
Channel_Inf chan_inf[NUM_PATH] = {{0.1, 50, 100}, {0.2, 90, 50}};
//Tile_Num tile_num{TILE_NUM, FOV_TILE_NUM, 
//	              CUSHION_TILE_NUM, OUTMOST_TILE_NUM};
int tile_num[REGION_NUM] = {FOV_TILE_NUM, CUSHION_TILE_NUM, 
							 OUTMOST_TILE_NUM};
//the unit is Mb/s
double _bitrate[BITRATE_TYPE_NUM] = {50, 25, 10};

int main() {
    int flag_video = 0;
//    std::string _input;
	int id_VSegment = 0;

    std::ifstream File;
    std::string inString;

    Data_Manager data_manager(100);
	FEC_Param_Adjuster fec_param_adj;
	Bitrate_Selector bitrate_selector(_bitrate);
	Video_Reader video_reader;	
	Path_Selector path_selector;

	fec_param_adj.setFEC_params(chan_inf, bitrate_selector, video_reader);
	bitrate_selector.setBitrate(tile_num, chan_inf, video_reader);
	path_selector.select_Path(chan_inf, video_reader);

    File.open("../../../video_test/machu_picchu_a_s111_non_B.265", std::ios::in);
//    File.open("../../../machu_picchu_8k_a_s111.265", std::ios::in);
//    File.open("input_non_b.265", std::ios::in);
//	std::thread readVideo_worker(&Video_Reader::video_reader_td_func,
//								 &video_reader, data_manager,
//								 id_VSegment);
    inString = slurp(File);

    flag_video = hevc_parser(inString, 1);

//    readVideo_worker.join();

    return 0;    
}
#endif

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