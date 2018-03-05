#include "../include/video_reader.h"
#include "../include/bitrate_selector.h"


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180305
//Description: the thread function which implement the FEC params decision  
//Parameter:   plr[] denotes the inputed packet loss rate, which is feedback
//             both Bitrate_Selectoris and Video_Reader are friend classes
//             S_FEC is equal to the encoding symbol size 
//             K_FEC denotes the orignal block size(K)
//==========================================================================
setFEC_param(double plr[], Bitrate_Selector &bitrate_selector,
			 Video_Reader &video_reader) {
	for(int i = 0; i < NUM_PATH; i++) {
		bitrate_selecotor.S_FEC = 1000;
		bitrate_selector.K_FEC  = 224;
		video_reader.S_FEC = 1000;
		video_reader.K_FEC = 224;
}
//==========================================================================
