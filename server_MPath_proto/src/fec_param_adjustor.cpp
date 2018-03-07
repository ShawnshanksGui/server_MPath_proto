#include "../include/video_reader.h"
#include "../include/bitrate_selector.h"

#include "math.h"

FEC_Param_Adjuster::FEC_Param_Adjuster() {
	for(int i = 0; i < 6; i++)
		avail_K[i] = 256 - (uchar)pow(2, i+1);  
}

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180305
//Description: the thread function which implement the FEC params decision  
//Parameter:   plr[] denotes the inputed packet loss rate, which is feedback
//             both Bitrate_Selectoris and Video_Reader are friend classes
//             S_FEC is equal to the encoding symbol size 
//             K_FEC denotes the orignal block size(K)
//==========================================================================
void FEC_Param_Adjuster::setFEC_param(Channel_Inf chan_inf[NUM_PATH], 
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
	double bw_total = chan_inf[0].avail_bw + chan_inf[1].avail_bw;
	double plr_estimated = chan_inf[0].plr*(chan_inf[0].avail_bw/bw_total)+
		 				   chan_inf[1].plr*(chan_inf[1].avail_bw/bw_total);

	for(int i = 0; i < 6; i++) {
		if(plr_estimated <= (256.0-avail_K[i])/(double)256.0) {
			video_reader.K_FEC[][] = avail_K[i];
		}
	}
}
//==========================================================================