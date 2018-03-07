#include "../include/system_params.h"
#include "../inlcude/bitrate_select.h"
#include "../include/common.h"

Bitrate_Selector::setBitrate(int _bitrate[BITRATE_TYPE_NUM]) {
	for(int i = 0; i < BITRATE_TYPE_NUM; i++) {
		bitrate[i] = _bitrate[i];
	}
}

void Bitrate_Selector::setBitrate(int tile_num[], Channel_Inf chan_inf, 
								  Video_Reader &video_reader) {
	double bw_redsidual;

	for(int k = 0; k < REGION_NUM; k++)
		video_reader.bitrate_decs[k] = bitrate[LOWEST];	

	bw_residual = chan_inf.avail_bw*(1-chan_inf.plr) - 
				  bitrate[LOWEST];
	if(bw_residual < (bitrate[MEDIAN]/TILE_NUM_TOTAL*tile_num[1])) {
		return;
	}
//start from FOV region, which feature a higher priority to outmostregion
	for(int i = 0; i <= REGION_NUM; i++){
//start from the higher quality
		for(int k = 1; k < BITRATE_TYPE_NUM; k++) {
			if(bw_residual > (bitrate[k] / TILE_NUM_TOTAL * tile_num[i]))
				video_reader.bitrate_decs[i] = bitrate[k];
		}
	}
}