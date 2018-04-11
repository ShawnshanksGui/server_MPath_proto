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

//server to receiver
#define START 1
#define END   0
#define STOP  0
//client to server
#define REQUEST 1 


using namespace std;


//notice whether ought to close the corresonding threads or not
//global external varible for threads' start and stop flag
int Terminal_AllThds = 0;
int Terminal_SendThds = 0;
//int Terminal_EncdThds = 0;
int Terminal_ReaderThds = 0;


//two channels' realtime infomation
Channel_Inf chan_inf[NUM_PATH] = {{0.01, 50.0, 30.0}, {0.03, 90.0, 10.0}};
//Tile_Num tile_num{TILE_NUM, FOV_TILE_NUM, 
//	              CUSHION_TILE_NUM, OUTMOST_TILE_NUM};
int tile_num[REGION_NUM] = {FOV_TILE_NUM, CUSHION_TILE_NUM, 
							 OUTMOST_TILE_NUM};
//the unit is Mb/s
double _bitrate[BITRATE_TYPE_NUM] = {100.0, 25.0, 6.0};


int main(int argc, char **argv) {
	srand((unsigned int)time(NULL)); 
//specify which one is the current video segmentation 
	int Num_VSegment = 1;

//the one control subflow	
	Transmitter server_ctrlSubflow;
	server_ctrlSubflow.transmitter_new_tcp(argv[9], argv[10]);

	for(int id_seg = 0; id_seg < Num_VSegment; id_seg++) {

		char pkt_recv[10] = {'\0'};
		server_ctrlSubflow.Recv_tcp(pkt_recv, 10);
//		server_ctrlSubflow.Recv_tcp_fixed_len(pkt_recv, 10);
		if(pkt_recv[0] == REQUEST) {
			printf("recv a video request packet\n");
			char pkt_ctrl[10] = {'\0'};	
			pkt_ctrl[0] = START;		
			server_ctrlSubflow.Send_tcp(pkt_ctrl, 10);
			printf("send a ctrl *START* packet\n");
			//inform all the threads of starting
			Terminal_AllThds = NO;

//===============================================================================
//class arguments!!!			
			Timer t;
			FEC_Param_Adjuster fec_param_adj;
			Bitrate_Selector bitrate_selector(_bitrate);
			Path_Selector path_selector;

//			vector<Encoder> encoder(NUM_PATH);
//the two data subflows!!!

			vector<Transmitter> server_dataSubflow(NUM_PATH);

			server_dataSubflow[0].transmitter_new_tcp(argv[1],argv[2]);
//			server_dataSubflow[1].transmitter_new_tcp(argv[5],argv[6]);	
//================================================================================
//reset the corresponding classes
			Data_Manager data_manager;
			Video_Reader video_reader;

			fec_param_adj.setFEC_params(chan_inf, bitrate_selector, video_reader);
			bitrate_selector.setBitrate(tile_num, chan_inf, video_reader);
			path_selector.select_Path(chan_inf, video_reader);
//================================================================================
//create corresponding threads!!!
			thread setTimer_worker(&Timer::setTimer_td_func, &t);

			std::thread readVideo_worker(&Video_Reader::video_reader_td_func,
										 &video_reader, ref(data_manager),
								 		 id_seg+1);
//			vector<thread> encoder_worker(NUM_PATH);
			vector<thread> sender_worker(NUM_PATH);

			for(int i = 0; i < 1; i++) {
//			for(int i = 0; i < NUM_PATH; i++) {
//				encoder_worker[i] = thread(&Encoder::encoder_td_func,
//								 		   &(encoder[i]), ref(data_manager));			
				sender_worker[i]  = thread(&Transmitter::send_td_func,
					                	   &server_dataSubflow[i], i, ref(data_manager));
			}
//reap or recycle the threads created.
			readVideo_worker.join();

			for(int i = 0; i < 1; i++) {
//			for(int i = 0; i < NUM_PATH; i++) {
//				encoder_worker[i].join();
				sender_worker[i].join();
			}
			setTimer_worker.join();
		}			
	}

	return 0;
}	


/*
int main(int argc, char **argv) {
	Transmitter server;
//	server.transmitter_new_tcp_non_b(argv[1], argv[2]);
//	server.transmitter_new_tcp_non_b(argv[5], argv[6]);
//	server.transmitter_new_tcp_non_b(argv[9], argv[10]);
	server.transmitter_new_tcp(argv[9], argv[10]);

//	char *data[4] = {"33333344444","55555566666","77777788888","88888899999"}; 
	int k = 0;
	int i = 0;
	long int cnt_bytes = 0;
	long int cnt_pkts  = 0;

	char packet[1000] = {'\0'};

	char str_test[1000] = {'\0'};

    Timer t;
    thread setTimer_worker(&Timer::setTimer_td_func, &t);

	for(k = 0;  k < 100000; k++) {
	for(i = 0; i < 500; i++) {
        
        if(Terminal_AllThds) {
            break;
        }		

		memset(str_test, 0, 1000);
		for(int k = 0; k < 1000; k++) {
			str_test[k] = i%9+1+67;
		}
//		cnt_bytes += server.Send_tcp_non_b(str_test, 1000);
		cnt_bytes += server.Send_tcp(str_test, 1000);
		cnt_pkts++;
//		printf("\nthe %d-th sent packet is %s\n", i, str_test);
//		printf("\nthe %d-th sent packet\n", i);

//		memset(packet, 0, 1000);
//		server.Recv_tcp(packet, 1000);
//		printf("\nthe %d-th recv packet is %s\n", i, packet);
//		printf("\nthe %d-th recv packet\n", i); 

//		server.Send_tcp(data[i], 11);
//		printf("\nthe sent packet is %s\n", data[i]);
//		server.Recv_tcp(packet, 11);
//		printf("\nthe recv packet is %s\n", packet);
	}
//	usleep(10000);
	}
//	packet[0]
	printf("\nsend total %d packets, %dbytes\n", cnt_pkts, cnt_bytes);
	server.Send_tcp_non_b("\0", 1);

    setTimer_worker.join();

	return 0;
}

*/