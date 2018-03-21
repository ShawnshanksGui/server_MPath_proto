#include <chrono>
#include <string>
#include <vector>
#include <thread>

#include "../include/common.h"
#include "../include/video_reader.h"
#include "../include/data_manager.h"
#include "../include/system_params.h"
#include "../include/path_selector.h"
#include "../include/codeStreaming_parser.h"

#include "../include/myUtility.h"
#include "../include/bitrate_select.h"
#include "../include/fec_param_adjustor.h"


//#ifdef ENABLE_DEBUG_READER

//two channels' realtime infomation
Channel_Inf chan_inf[NUM_PATH] = {{0.03, 50.0, 100.0}, {0.05, 90.0, 50.0}};
//Tile_Num tile_num{TILE_NUM, FOV_TILE_NUM, 
//	              CUSHION_TILE_NUM, OUTMOST_TILE_NUM};
int tile_num[REGION_NUM] = {FOV_TILE_NUM, CUSHION_TILE_NUM, 
							 OUTMOST_TILE_NUM};
//the unit is Mb/s
double _bitrate[BITRATE_TYPE_NUM] = {50.0, 25.0, 10.0};

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

//    video_reader.setVideoReader(std::move(data_manager));

//    std::thread readVideo_worker(&(video_reader.video_reader_td_func),
//								 data_manager,id_VSegment);
	std::thread readVideo_worker(&Video_Reader::video_reader_td_func,
								 &video_reader, std::ref(data_manager),
								 id_VSegment);

//    std::thread readVideo_worker(&Video_Reader::video_reader_td_func,
 //   							 &video_reader, id_VSegment);

//    inString = slurp(File);
//    flag_video = hevc_parser(inString, 1);
    readVideo_worker.join();

    return 0;    
}
//#endif