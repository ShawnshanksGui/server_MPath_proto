#include <chrono>
#include <string>
#include <vector>
#include <thread>
#include <memory>

#include "../include/common.h"
#include "../include/video_reader.h"
#include "../include/data_manager.h"
#include "../include/path_selector.h"
#include "../include/codeStreaming_parser.h"
#include "../include/system_params.h"

//for debugging
//#define ENABLE_DEBUG_READER

using namespace std;

#ifdef  ENABLE_DEBUG_READER
#include "../include/myUtility.h"
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

	for(int i = 0; i < REGION_NUM; i++) {
		bitrate_decs[i] = -1;
	}
	for(int i = 0; i < REGION_NUM; i++) {
		for(int k = 0; k < FRAME_GOP*GOP_NUM; k++) 
			path_decs[i][k] = -1;
	}
}

void Video_Reader::reader_init() {
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
	
    int id_video = 1;
    int len_seg  = 1;

    std::ifstream File;
    std::string inString;


	printf("entering video_reader_td_func\n");
//	for(int i = 0; i < REGION_NUM; i++) {
	for(int i = 0; i < 1; i++) {    
		std::string inputVideo_Path;	

		printf("\n**in video_reader_td_func**, %d\n", this->bitrate_decs[i]);

//		inputVideo_Path = /*"v_1_" + */"/home/guifei/video_test/360video/v"\
//						  + to_string(id_video) + "/" + to_string(len_seg) \
//						  + "s/seg_" + to_string(id_VSegment) + "region_"\
//						  + to_string(i) + "/level_" \
//						  + to_string(this->bitrate_decs[i]) + ".265";
		inputVideo_Path = "/home/guifei/video_test/360video/v1/1s/seg_1/region_0/level_0.265";

//==========================================================================
   		File.open(inputVideo_Path, std::ios::in);
   		inString = slurp(File);
//replicate data to a safe location
   		VData_Type *cstr=new VData_Type[inString.length()+1];
   		strcpy(cstr, inString.c_str());

   		data_manager.data_vec.push_back(cstr);

    	hevc_parser(inString, i, this);

    	partition_nalu(i, cstr, id_VSegment, data_manager);
    }    
}
//==========================================================================	


//void Video_Reader::setVideoReader(Data_Manager &&d_manager) {	
//	data_manager.Data_Manager() 
//}

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180408
//Description: split the video segment into data block with equal length(S*K)  
//Parameters:  result stored into the Data_Mannager.queue<struct Elem_Data*>
//==========================================================================
void Video_Reader::partition_nalu(int id_region, VData_Type *p_str, int id_seg,
			 					  Data_Manager &data_manager) {
 	int prev_k_fec = VAL_FOR_INIT;
	int prev_m_fec = VAL_FOR_INIT;

	int cnt_bundle = -1;
	int bundle_arr[FRAME_GOP*GOP_NUM] = {0};

#ifdef ENABLE_FFT_RS
//==========================================================================	
	for(int i = 0; i < FRAME_GOP*GOP_NUM; i++) {
		if(0 == (i % FRAME_GOP)) {
			cnt_bundle++;
			bundle_arr[cnt_bundle] = 1;

			continue;
		}
		
		if((K_FEC[id_region][i%FRAME_GOP] != prev_k_fec) || \
		   (M_FEC[id_region][i%FRAME_GOP] != prev_m_fec)) {
		   	cnt_bundle++;
			bundle_arr[cnt_bundle]++;
			prev_k_fec = K_FEC[id_region][i%FRAME_GOP];
			prev_m_fec = M_FEC[id_region][i%FRAME_GOP];
		} 
		else {
			bundle_arr[cnt_bundle]++;
		}
	}
	cnt_bundle++;
//==========================================================================

#else
//==========================================================================
//make a bundle for every RATE frames
	for(int i = 0; i < FRAME_GOP*GOP_NUM; i++) {
		if(0 == (i%RATE)) {
			bundle_arr[i/RATE] = RATE;
		}
	}
	cnt_bundle = FRAME_GOP*GOP_NUM/RATE;
//==========================================================================
#endif	

//for debugging
	printf("this is the bundle_array, the number of total bundle is %d, \
			as following:\n", cnt_bundle);
	for(int i = 0; i < cnt_bundle; i++) {
		printf("%d ", bundle_arr[i]);
	}
	printf("\n");

	for(int id_bundle = 0, cnt_frame = 0; id_bundle < cnt_bundle; id_bundle++) {
//the len of the bundle of frames 
		int size = 0;

		for(int i = 0; i < bundle_arr[id_bundle]; i++) {
			size += nalu[id_region][cnt_frame+i]._size;
		}

		int location = nalu[id_region][cnt_frame]._addr;	

		int s_fec = S_FEC[id_region][cnt_frame%FRAME_GOP];
		int k_fec = K_FEC[id_region][cnt_frame%FRAME_GOP];
		int m_fec = M_FEC[id_region][cnt_frame%FRAME_GOP];
//the remainder number 
		int len_remaining = size % (s_fec*k_fec);
//the quotient  number
		int num = size / (s_fec*k_fec);

		p_str += nalu[id_region][cnt_frame]._addr;	

//when the length of nalu is divisible(num-1 == k) by 
//the product of s_fec and k_fec
		if(0 == len_remaining) {
//when the current nalu is longer than block size(s_fec*k_fec).
			for(int k = 0; k < num; k++){
				shared_ptr<struct Elem_Data> elem_data = (shared_ptr<struct \
											Elem_Data>)new(struct Elem_Data);
				if(0 == cnt_frame ) {
					if(0 == k)
						elem_data->type_location = FRONT_FOR_FIRST_NALU;
					else if (num-1 == k) //not divisible
						elem_data->type_location = END_FOR_FIRST_NALU;
					else {elem_data->type_location = MID_FOR_FIRST_NALU;}
				}
				else if(FRAME_GOP*GOP_NUM-1 == cnt_frame) {
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
				elem_data->id_seg = id_seg;
//here, i is nonsense, temporarily
				assign_attributes(elem_data, path_decs[id_region][cnt_frame], s_fec,
								  k_fec, m_fec, cnt_frame, location, p_str, data_manager);
				location += s_fec*k_fec;
			}
		}
//when the length of nalu is not divisible, (num == k)
		else {
			for(int k = 0; k <= num; k++) {
				shared_ptr<struct Elem_Data> elem_data = (shared_ptr<struct \
											Elem_Data>)new(struct Elem_Data);
//				shared_ptr<struct Elem_Data> elem_data(new(struct Elem_Data));

				if(0 == cnt_frame ) {
					if(0 == k)
						elem_data->type_location = FRONT_FOR_FIRST_NALU;

					else if (num == k) // divisible
						elem_data->type_location = END_FOR_FIRST_NALU;
					else {elem_data->type_location = MID_FOR_FIRST_NALU;}
				}
				else if(FRAME_GOP*GOP_NUM-1 == cnt_frame) {
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
					printf("the size = %d, the location = %d", size, location);
					elem_data->size = len_remaining;
				}
				else {elem_data->size = s_fec*k_fec;}
				elem_data->id_seg = id_seg;
//here, i is nonsense, temporarily
				assign_attributes(elem_data, path_decs[id_region][cnt_frame], s_fec,
				 				  k_fec, m_fec, cnt_frame, location, p_str, data_manager);
				location += s_fec*k_fec;
			}
		}

		cnt_frame += bundle_arr[id_bundle];
	}
}
//==========================================================================


//attention: the id_nalu is actually equal to the cnt_frame(the 
//starting frame of a frame bundle)
void Video_Reader::assign_attributes(shared_ptr<struct Elem_Data> elem_data,
									int path, int s_fec, int k_fec, int m_fec, 
									int id_nalu, int _addr, VData_Type *p_str,
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
	elem_data->M_FEC   = m_fec;

	data_manager.data_save(elem_data, elem_data->id_path);

	printf("this is result of video reader, as follwong:\n");
//	printf("id_nalu = %d , id_seg = %d, id_region = %d, type_nalu = %d, \
//		   S_FEC = %d, K_FEC = %d, M_FEC = %d\n\n", data_manager.data_video[path].front()->id_nalu, \
//		   data_manager.data_video[path].front()->id_seg, data_manager.data_video[path].front()->id_region, \
//		   data_manager.data_video[path].front()->type_nalu, data_manager.data_video[path].front()->S_FEC, \
//		   data_manager.data_video[path].front()->K_FEC, data_manager.data_video[path].front()->M_FEC);
	printf("id_nalu = %d , id_seg = %d, id_region = %d, type_nalu = %d, \
		   S_FEC = %d, K_FEC = %d, M_FEC = %d, size = %d\n\n", elem_data->id_nalu,\
		   elem_data->id_seg, elem_data->id_region, elem_data->type_nalu, \
		   elem_data->S_FEC, elem_data->K_FEC, elem_data->M_FEC, elem_data->size);
}



#ifdef ENABLE_DEBUG_READER
//two channels' realtime infomation
Channel_Inf chan_inf[NUM_PATH] = {{0.03, 50.0, 3.0}, {0.05, 90.0, 10.0}};
//Tile_Num tile_num{TILE_NUM, FOV_TILE_NUM, 
//	              CUSHION_TILE_NUM, OUTMOST_TILE_NUM};
int tile_num[REGION_NUM] = {FOV_TILE_NUM, CUSHION_TILE_NUM, 
							 OUTMOST_TILE_NUM};
//the unit is Mb/s
double _bitrate[BITRATE_TYPE_NUM] = {100.0, 25.0, 6.0};


int main() {
//    std::string _input;
	int id_VSegment = 1;

    Data_Manager data_manager;
	FEC_Param_Adjuster fec_param_adj;
	Bitrate_Selector bitrate_selector(_bitrate);
	Video_Reader video_reader;	
	Path_Selector path_selector;

	fec_param_adj.setFEC_params(chan_inf, bitrate_selector, video_reader);
	bitrate_selector.setBitrate(tile_num, chan_inf, video_reader);

	printf("\nin main dunction, the bitrate result is as following:\n"); 
	for(int i = 0; i < REGION_NUM; i++) {
		printf("%d ", video_reader.bitrate_decs[i]);
	}

	path_selector.select_Path(chan_inf, video_reader);
//create the threads required.
	std::thread readVideo_worker(&Video_Reader::video_reader_td_func,
								 &video_reader, ref(data_manager),
						 		 id_VSegment);

    readVideo_worker.join();

    return 0;    
}

#endif



void Video_Reader::video_reader_func(Data_Manager &data_manager,
									 int id_VSegment) {
/*
	int flag_video = 0;

    std::ifstream File;
    std::string inString;
	
//	for(int i = 0; i < REGION_NUM; i++) {
	for(int i = 0; i < 1; i++) {    
		std::string inputVideo_Path;	
//		inputVideo_Path = "video_????" + std::to_string(bitrate_decs[i]) +
//		             "_" + std::to_string(id_VSegment) + ".265";
//		inputVideo_Path = "../../../video_test/machu_picchu_a_s111_non_B.265";
		inputVideo_Path = "input_non_b.265";
//==========================================================================
   		File.open(inputVideo_Path, std::ios::in);
   		inString = slurp(File);
//replicate data to a safe location
   		VData_Type *cstr = new VData_Type[inString.length() + 1];
   		strcpy(cstr, inString.c_str());

   		data_manager.data_vec.push_back(cstr);

    	hevc_parser(inString, i, this);

    	partition_nalu(i, cstr, id_VSegment, data_manager);
//    	delete [] cstr;
    }
*/
}


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180312
//Description: split the video segment into data block with equal length(S*K)  
//Parameters:  result stored into the Data_Mannager.queue<struct Elem_Data*>
//==========================================================================
/*
void Video_Reader::partition_nalu(int id_region, VData_Type *p_str, int id_seg,
			 					  Data_Manager &data_manager) {
//	const VData_Type *p_str = inString.c_str();
	for(int i = 0; i < FRAME_GOP*GOP_NUM; i++) {
//get the start address for the current nalu(or frame);
		int location = nalu[id_region][i]._addr;

		int s_fec = S_FEC[id_region][i%FRAME_GOP];
		int k_fec = K_FEC[id_region][i%FRAME_GOP];
		int m_fec = M_FEC[id_region][i%FRAME_GOP];

//remainder number 
		int len_remaining = (nalu[id_region][i]._size) % (s_fec*k_fec);
//
		int num = nalu[id_region][i]._size / (s_fec*k_fec);

//real start address of current NALU(or frame) in inString data.
		p_str = p_str + nalu[id_region][i]._addr;

//when the length of nalu is divisible(num-1 == k) by 
//the product of s_fec and k_fec
		if(0 == len_remaining) {
//when the current nalu is longer than block size(s_fec*k_fec).
			for(int k = 0; k < num; k++){
				shared_ptr<struct Elem_Data> elem_data = (shared_ptr<struct \
											Elem_Data>)new(struct Elem_Data);
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
				elem_data->id_seg = id_seg;

				assign_attributes(elem_data, path_decs[id_region][i], s_fec,
								  k_fec, m_fec, i, location, p_str, data_manager);
				location += s_fec*k_fec;
			}
		}
//when the length of nalu is not divisible, (num == k)
		else {
			for(int k = 0; k <= num; k++) {
				shared_ptr<struct Elem_Data> elem_data = (shared_ptr<struct \
											Elem_Data>)new(struct Elem_Data);
//				shared_ptr<struct Elem_Data> elem_data(new(struct Elem_Data));

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
				elem_data->id_seg = id_seg;

				assign_attributes(elem_data, path_decs[id_region][i], s_fec,
				 				  k_fec, m_fec, i, location, p_str, data_manager);
				location += s_fec*k_fec;
			}
		}
	}
}
*/
//==========================================================================