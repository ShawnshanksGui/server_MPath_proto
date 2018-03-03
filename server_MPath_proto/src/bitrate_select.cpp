#include "../inlcude/bitrate_select.h"

void Bitrate_Select::bitrate_select_thread_fun(Video_Reader &video_reader) {
	for(int i = 0; i < video_reader.region_num; i++){
		video_reader.bitrate_decs[region_num] = 1;
	}
}