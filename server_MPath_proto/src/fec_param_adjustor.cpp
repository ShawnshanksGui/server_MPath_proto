#include "../include/fec_param_adjustor.h"
#include "../include/video_reader.h"
#include "../include/bitrate_select.h"
#include "../include/system_params.h"

#include "../include/common.h"

#include "math.h"

#define ENABLE_DEBUG_FEC_ADJUSTOR

FEC_Param_Adjuster::FEC_Param_Adjuster() {
	for(int i = 0; i < LEVEL_NUM; i++)
		avail_K[i] = 256 - (int)pow(2, i+1);  
}

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180305
//Description: the thread function which implement the FEC params decision  
//Parameter:   plr and avail_bw denote the packet loss rate and avalable 
//			   bandwidth in realtime,respectively,both of which are feedback.
//             both Bitrate_Selectoris and Video_Reader are friend classes
//             S_FEC is equal to the encoding symbol size(S)
//             K_FEC denotes the orignal block size(K)
//==========================================================================
void FEC_Param_Adjuster::setFEC_params(Channel_Inf chan_inf[NUM_PATH], 
									  Bitrate_Selector &bitrate_selector,
			 						  Video_Reader &video_reader) {
/*
	for(int i = 0; i < NUM_PATH; i++) {
		bitrate_selector.S_FEC = 1000;
		bitrate_selector.K_FEC  = 224;
		video_reader.S_FEC = 1000; 
		video_reader.K_FEC = 224;
	}
*/
	int indicator = 0;

	double bw_total = chan_inf[0].avail_bw + chan_inf[1].avail_bw;
	double plr_estimated = chan_inf[0].plr*(chan_inf[0].avail_bw/bw_total)+
		 				   chan_inf[1].plr*(chan_inf[1].avail_bw/bw_total);

	for(int i = 0; i < LEVEL_NUM; i++) {
		if(plr_estimated <= (256.0-avail_K[i])/(double)256.0) {
			video_reader.K_FEC[REGION_NUM-1][FRAME_GOP-1] = avail_K[i];
			indicator = i;
			break;
		}
//K_FEC = 128 if exceedings the range of available redundancy rate
		if(LEVEL_NUM-1 == i) {
			video_reader.K_FEC[REGION_NUM-1][FRAME_GOP-1] = 
										avail_K[LEVEL_NUM-1];
			indicator = LEVEL_NUM-1;
		}
	}

	for(int k = REGION_NUM-1; k >= 0; k--) {	
		for(int i = FRAME_GOP-1; i >= 0; i--) {
			if((FRAME_GOP-1)==i && (REGION_NUM-1)==k) continue;
//decide the K(or redundancy rate start from the last frame)
			if(((indicator+(FRAME_GOP-1-i)/STEP_RATE) + (REGION_NUM-1-k)) < LEVEL_NUM) { 		
				video_reader.K_FEC[k][i]=avail_K[indicator+(FRAME_GOP-1-i)/STEP_RATE +
										 (REGION_NUM-1-k)];
			}
			else {
//if expected redundancy rate exceeds the available range of rate.
				video_reader.K_FEC[k][i]=avail_K[LEVEL_NUM-1];
			}
/*
			else if((indicator+REGION_NUM-1-k) > 6) {
			//if expected redundancy rate exceeds the available range of range.
				video_reader.K_FEC[k][i]=avail_K[6];
			}
*/
		}	
	}

#ifdef ENABLE_DEBUG_FEC_ADJUSTOR
	printf("\nThe k_value is as following:\n");
	for(int i = 0; i < LEVEL_NUM; i++) {
		printf("%d ", avail_K[i]);
	}

	printf("\nThe FEC parameters are as following:\n");
	for(int i = 0; i < REGION_NUM; i++) {
		for(int k = 0; k < FRAME_GOP; k++) {
			printf("%d ", video_reader.K_FEC[i][k]);
		}
	}
#endif
}
//==========================================================================