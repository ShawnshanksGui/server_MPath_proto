#include "../include/system_params.h"
#include "../inlcude/bitrate_select.h"
#include "../include/common.h"

void Bitrate_Selector::setBitrate(Tile_Num tile_num, Channel_Inf chan_inf, 
								  Video_Reader &video_reader) {
	int tmp = 0;
	double bw_redsidual = 0.0;

	for(int i = 0; i < video_reader.region_num; i++){
		int tmp = 0;

		bw_residual = chan_inf.avail_bw - bitrate[LOWEST] * NUM_FILE;
		if(bw_residual < bitrate[MEDIAN]*NUM_FOV_TILE) {
			for(int k = 0; k < video_reader.region_num; k++)
				video_reader.bitrate_decs[k] = tmp;
		}

		for(int k = 1; k < video_reader.num_bitrateType; k++) {
			if(bw_residual > bitrate[k]*NUM_FOV_TILE)
			video_reader.bitrate_decs[region_num] = 1;
		}
	}
}