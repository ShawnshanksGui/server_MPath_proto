//#include "thread_core_affinity_set.h"
#include <string>
#include <thread>
#include <mutex>

#include   "../include/server.h"
#include   "../include/common.h"
#include "../include/mySocket.h"
#include  "../include/utility.h"
//#include   "pthread.h"








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