//#include "thread_core_affinity_set.h"
#include <string>
#include <thread>
#include <mutex>

#include   "../include/server.h"
#include   "../include/common.h"
#include "../include/mySocket.h"
#include  "../include/utility.h"
//#include   "pthread.h"


int main() {
	double plr = 0.1;

	int startFlag_one_timeSlice = 0;

	Timer t;
	FEC_Param_Adjuster fec_param_adj;
	Bitrate_Selector bitrate_selector;
	Path_Selector path_selector;
	Video_Reader video_reader;
	Encoder encoder[NUM_PATH];

	Transmitter server[NUM_PATH];
	server[0].transmitter_new(argv[1], argv[2], argv[3], argv[4]);

	std::thread setTimer_worker(&Timer::setTimer_td_func, &t, 
		                        startFlag_one_timeSlice);	

	while(1) {
		if(startFlag_one_timeSlice) {
/*
		std::thread setFECParam_worker(&FEC_Param_Adjuster::setFECParam_td_func,
	    	                           &fec_param_adj);
		std::thread setBitrate_worker(&Bitrate_Selector::setBitrate_td_func,
								      &bitrate_selector);
		std::thread pathSelect_worker(&Path_Selector::setPath_td_func,
									  &path_selector);
*/		
			fec_param_adj.setFEC_param(plr, bitrate_selctor, video_reader);
			path_selector.setPath(video_reader);
			bitrate_selctor.setBitrate(video_reader);
//create the threads required.
			std::thread readVideo_worker(&Video_Reader::video_reader_thread,
										 &video_reader);
			for(int i = 0; i < NUM_PATH; i++) {
				std::thread encoder_worker(&Encoder::encoder_thread;
										   &encoder[i]);
				std::thread sender_worker(&Transmitter::send_thread, 
					                      server[i]);
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