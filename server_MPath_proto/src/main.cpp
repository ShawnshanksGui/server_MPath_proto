#include <chrono>
#include <string>
#include <vector>
#include <thread>

#include "fstream"
#include "sstream"
#include "iostream"
#include "ios"

#include "../include/timer.h"
#include "../include/common.h"
#include "../include/mySocket.h"
#include "../include/encoder.h"
#include "../include/video_reader.h"
#include "../include/data_manager.h"
#include "../include/system_params.h"
#include "../include/path_selector.h"
#include "../include/codeStreaming_parser.h"

#include "../include/myUtility.h"
#include "../include/bitrate_select.h"
#include "../include/fec_param_adjustor.h"

using namespace std;
/*
//two channels' realtime infomation
Channel_Inf chan_inf[NUM_PATH] = {{0.01, 50.0, 30.0}, {0.03, 90.0, 10.0}};
//Tile_Num tile_num{TILE_NUM, FOV_TILE_NUM, 
//	              CUSHION_TILE_NUM, OUTMOST_TILE_NUM};
int tile_num[REGION_NUM] = {FOV_TILE_NUM, CUSHION_TILE_NUM, 
							 OUTMOST_TILE_NUM};
//the unit is Mb/s
double _bitrate[BITRATE_TYPE_NUM] = {100.0, 25.0, 6.0};


int main(int argc, char **argv) {
//specify which one is the current video segmentation 
	int id_VSegment = 0;
//initialize the terminal flag as no valid 
	int terminalFlag = 0;
//inttialize the start flag as no valid
	int startFlag_one_timeSlice = 0;

	Timer t;

	FEC_Param_Adjuster fec_param_adj;
	Bitrate_Selector bitrate_selector(_bitrate);
	Path_Selector path_selector;
//	Video_Reader video_reader;

	vector<Encoder> encoder(NUM_PATH);
	vector<Transmitter> server(NUM_PATH);

//	vector<thread> encoder_worker(NUM_PATH);
	vector<thread> sender_worker(NUM_PATH);

	server[0].transmitter_new_udp(argv[1], argv[2], argv[3], argv[4]);
	server[1].transmitter_new_udp(argv[5], argv[6], argv[7], argv[8]);
	thread setTimer_worker(&Timer::setTimer_td_func, &t, 
		                   ref(startFlag_one_timeSlice), 
		                   ref(terminalFlag));	

	while(1) {
		//reset the key data struct
//		Video_Reader video_reader;
//		video_reader.reader_init();
//		Data_Manager data_manager;

//		if(terminalFlag > 0) {break;}
//		if(startFlag_one_timeSlice > 0) {

			//process the next video segment
			id_VSegment++;

//reset the corresponding classes
			Data_Manager data_manager;
			Video_Reader video_reader;

			fec_param_adj.setFEC_params(chan_inf, bitrate_selector, video_reader);
			bitrate_selector.setBitrate(tile_num, chan_inf, video_reader);
			path_selector.select_Path(chan_inf, video_reader);
//create the threads required.
			std::thread readVideo_worker(&Video_Reader::video_reader_td_func,
										 &video_reader, ref(data_manager),
								 		 id_VSegment);

			for(int i = 0; i < NUM_PATH; i++) {
//				encoder_worker[i] = thread(&Encoder::encoder_td_func,
//								 		   &(encoder[i]), ref(data_manager));			
				sender_worker[i]  = thread(&Transmitter::send_td_func, 
					                	   &server[i], i, ref(data_manager));
			}
			//reap or recycle the threads created.	
			readVideo_worker.join();	
			for(int i = 0; i < NUM_PATH; i++) {
//				encoder_worker[i].join();
				sender_worker[i].join();
			}	
//		}	
	}

//	setTimer_worker.join();

	return 0;
}	
*/
int main(int argc, char **argv) {
	Transmitter server;
	server.transmitter_new_tcp(argv[1], argv[2]);

	char *data[4] = {"33333344444","55555566666","77777788888","88888899999"}; 
	char packet[11] = {'\0'};

	for(int i = 0; i < 4; i++) {
		server.Send_tcp(data[i], 11);
		printf("\nthe sent packet is %s\n", data[i]);
		server.Recv_tcp(packet, 11);
		printf("\nthe recv packet is %s\n", packet);
	}
//	packet[0]
	server.Send_tcp("\0", 1);

	return 0;
}
