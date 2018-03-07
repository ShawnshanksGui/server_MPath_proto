#include "../include/system_params.h"
#include "../include/path_selector.h"
#include "../include/video_reader.h"

void Path_Selector::select_Path(Channel_Inf chan_inf[], 
								Video_Reader &video_reader) {
/*
	double bw_final[NUM_PATH];

	for(int i = 0; i < NUM_PATH; i++) {
		bw_final[i] = chan_inf.avail_bw * (1-chan_inf.plr);
	}
	for(int id_path; id_path < NUM_PATH; id_path++) {
		for(int i = 0; i < REGION_NUM; i++) {
			if(bw_final[id_path] > video_reader.bitrate_decs[i])
				for(int k = 0; k < FRAME_GOP; k++) {
					path_decs[k][i] = id_path; 
				}
			else {
				break;
			}
		}
	}
*/
		for(int i = 0; i < REGION_NUM; i++) {
			if(i < 2) {
				for(int k = 0; k < video_reader.frame_num; k++) {
					path_decs[k][i] = CELLULAR_NETWORK; 
			    }
			}
			else {
				for(int k = 0; k < FRAME_GOP; k++) {
					path_decs[k][i] = WIFI; 				
			}
		}
}