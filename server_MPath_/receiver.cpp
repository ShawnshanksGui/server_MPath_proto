//#define   _GNU_SOURCE
//#include "sys/syscall.h"

//#include "thread_core_affinity_set.h"
#include    "receiver.h"
#include    "common.h"
#include   "mySocket.h"
#include   "utility.h"
#include   "pthread.h"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
	int num_recv = 0;

	Usage(argc, NUM_PARA, argv[0]);
	char buf[RECLINE];
	Udp_sock _server;
	
	FILE *fp = NULL;

	_server.udp_sock_server_new(argv[1], argv[2]);

	Fopen_for_write(&fp, argv[3]);  //open file to be transmitted!

	while(1) {
		num_recv = _server.Recv_udp(buf, RECLINE);
//		printf("receive %d bytes      ", num_recv);
		if(0 == num_recv) {break;}
		Fwrite(buf, num_recv, fp);
	}

	return 0;
}
