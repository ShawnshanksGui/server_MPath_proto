//#include "thread_core_affinity_set.h"
#include <string>
#include <thread>
#include <mutex>

#include        "../include/server.h"
#include        "../include/common.h"
#include       "../include/utility.h"
#include      "../include/mySocket.h"
#include "../include/system_params.h"

//two channels' realtime infomation
Channel_Inf chan_inf[NUM_PATH] = {{0.1, 50, 100}, {0.2, 90, 50}};
//Tile_Num tile_num{TILE_NUM, FOV_TILE_NUM, 
//	              CUSHION_TILE_NUM, OUTMOST_TILE_NUM};
int tile_num [REGION_NUM] = {FOV_TILE_NUM, CUSHION_TILE_NUM, 
							 OUTMOST_TILE_NUM};
//the unit is Mb/s
int _bitrate[BITRATE_TYPE_NUM] = {50, 25, 10};


int main() {
	int startFlag_one_timeSlice = 0;

	Timer t;
	FEC_Param_Adjuster fec_param_adj;
	Bitrate_Selector bitrate_selector = Bitrate_Selector(_bitrate);
	Path_Selector path_selector;
	Video_Reader video_reader;
	Encoder encoder[NUM_PATH];

	Transmitter server[NUM_PATH];
	server[0].transmitter_new(argv[1], argv[2], argv[3], argv[4]);

	std::thread setTimer_worker(&Timer::setTimer_td_func, &t, 
		                        startFlag_one_timeSlice);	

	while(1) {
		if(startFlag_one_timeSlice) {

			fec_param_adj.setFEC_param(chan_inf, bitrate_selector, video_reader);
			bitrate_selector.setBitrate(tile_num, chan_inf, video_reader);
			path_selector.select_Path(chan_inf, video_reader);

//create the threads required.
			std::thread readVideo_worker(&Video_Reader::video_reader_td_func,
										 &video_reader);
			for(int i = 0; i < NUM_PATH; i++) {
				std::thread encoder_worker(&Encoder::encoder_td_func;
										   &encoder[i]);
				std::thread sender_worker(&Transmitter::send_td_func, 
					                      &server[i]);
			}
//recycle the threads created.	
			readVideo_worker.join();	
			for(int i = 0; i < NUM_PATH; i++) {
				encoder_worker.join();
				sender_worker.join();
			}	
		}	
	}

	return 0;
}

/*
		std::thread setFECParam_worker(&FEC_Param_Adjuster::setFECParam_td_func,
	    	                           &fec_param_adj);
		std::thread setBitrate_worker(&Bitrate_Selector::setBitrate_td_func,
								      &bitrate_selector);
		std::thread pathSelect_worker(&Path_Selector::setPath_td_func,
									  &path_selector);
*/		

/*
int main(int argc, char *argv[]) {	
	int num_sent = 0;
	
	FILE *fp = NULL;
	char buf[CODELINE] = {'\0'};

	Usage(argc, NUM_PARA, argv[0]);

	Udp_sock a_client;
	a_client.udp_sock_client_new(argv[1], argv[2], argv[3], argv[4]);

	Fopen_for_read(&fp, argv[5]);  //open file to be transmitted!

	while(1) {
		num_sent = Fread(buf, CODELINE, fp);
		usleep(10);
//		printf("send %d bytes   ", num_sent);
		if(END_FILE == num_sent) {
			a_client.Send_udp(buf, num_sent);
			break;
		}
		a_client.Send_udp(buf, num_sent);
	}

	return 0;
}
*/