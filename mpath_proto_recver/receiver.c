#define   _GNU_SOURCE
#include "sys/syscall.h"

#include "thread_core_affinity_set.h"

#include   "common.h"
#include  "_socket.h"
#include  "utility.h"
#include "receiver.h"
#include  "pthread.h"

//#include "sys/syscall.h"

void *transmit_new(char *_type)
{
	struct Transmit* _transmit = (struct Transmit *)malloc(sizeof(struct Transmit));
	
	if (!strcmp(_type, "UDP")){
		_transmit->type_proto = UDP;
		_transmit->socket_new = udp_sock_recver_new;
//		_transmit->_send      = Send_udp;
		_transmit->_recv      = Recv_udp;
	}
	else if (!strcmp(_type, "TCP")){
		_transmit->type_proto = TCP;
		_transmit->socket_new = tcp_sock_recver_new;
//		_transmit->_send      = Send_tcp;
		_transmit->_recv      = Recv_tcp;       
	}
	else {
		perror("can't match type of transport protcol!");
		exit(0);
	}

	return _transmit; 
}

/*
int main(int argc, char **argv)
{
	struct timeval starttime;
	struct timeval endtime;
	double time_past = 0.0; 	

	int recv_len   = 0;
	char ctp_buf[RECLINE] = {'\0'};

	int flag_receive = 0;   
	int count_flag = 0;

	Usage(argc, 4, argv[0]);

	struct Transmit *_transmit = transmit_new(argv[1]);     //argv[1] ---> type of transport protocol

	struct Socket *_socket = _transmit->socket_new(argv[2], argv[3]);  

	while (1) {
		if ((recv_len = _transmit->_recv(_socket, ctp_buf, RECLINE)) > 0) {
			if (!flag_receive) {
				flag_receive = 1;
				gettimeofday(&starttime, 0);
			}
//			bzero(ctp_buf, sizeof(ctp_buf));
			recv_len = 0;	

			count_flag++;
			if (AMOUNT_MAX == count_flag) {
				gettimeofday(&endtime, NULL);
				printf("count: %d\n", count_flag);
				time_past = 1000000*(endtime.tv_sec-starttime.tv_sec) + (endtime.tv_usec-starttime.tv_usec);
				printf("all packet finish receiving, the time of cost is %lfus\n", time_past);
				break;
			}
		}	
	}
	
	close_socket(_socket);
	free(_transmit);
	free(_socket);

	exit(0);
}
*/
void *thread_work(void *args_work) {
	struct timeval starttime;
	struct timeval endtime;
	double time_past = 0.0; 	

	int recv_len   = 0;
	char ctp_buf[RECLINE] = {'\0'};

	int flag_receive = 0;   
	int count_pkt   = 0;
	int byte_receive = 0;

	struct Args *args = (struct Args *)args_work;
	struct Transmit *_transmit = transmit_new(args->type_proto_str); //pass type of transport protocol
	struct Socket *_socket = _transmit->socket_new(args->addr_self_str,
                                                   args->port_self_str); 
//set affinity between threads and cores 
	affinity_set(args->id_core);

	while (1) {
		if ((recv_len = _transmit->_recv(_socket, ctp_buf, RECLINE)) > 0) {
			if (!flag_receive) {
				flag_receive = 1;
				gettimeofday(&starttime, 0);
			}
//			bzero(ctp_buf, sizeof(ctp_buf));
			byte_receive += recv_len;
			recv_len = 0;	
		
			count_pkt++;
/*
			if (AMOUNT_MAX == count_flag) {
				gettimeofday(&endtime, NULL);
				printf("count: %d\n", count_flag);
				time_past = 1000000*(endtime.tv_sec-starttime.tv_sec) + (endtime.tv_usec-starttime.tv_usec);
				printf("all packet finish receiving, the time of cost is %lfus\n", time_past);
				break;
			}
*/
		}
		else {
        	break; 
		}	
	}

	printf("received %d packets!!!\n", count_pkt);

	pid_t id_thread_child = syscall(SYS_gettid);
	printf("\nID of the child thread is %d\n", id_thread_child);
	
	gettimeofday(&endtime, NULL);
	time_past = 1000000*(endtime.tv_sec-starttime.tv_sec) + (endtime.tv_usec-starttime.tv_usec);	
	printf("all packet finish receiving, received %lf Mbytes\n,\
            the time of cost is %lf us\n, the rate of receiving is %lf Mb/s\n", 
            (double)byte_receive/1000000.0, time_past, byte_receive/time_past);

	close_socket(_socket);
	free(_transmit);
	free(_socket);
}

int main(int argc, char *argv[]) {
	int       loct_command = 2;   // sign the location of command!
	pthread_t threads[TOTAL_WORK_THREADS]; 

	Usage(argc, 6, argv[0]);

    struct Args *args_work = (struct Args *)malloc(TOTAL_WORK_THREADS * sizeof(struct Args));

	for (int i = 0; i < TOTAL_WORK_THREADS; i ++) {
		args_work[i].id_core        = i;
		args_work[i].type_proto_str = argv[1];
		args_work[i].addr_self_str  = argv[loct_command++];
		args_work[i].port_self_str  = argv[loct_command++];
		pthread_create(&threads[i], NULL, thread_work, (void *)&args_work[i]);
	}

	for (int i = 0; i < TOTAL_WORK_THREADS; i ++) {
        pthread_join(threads[i], NULL);
    }         

   	free(args_work);

	return 0;
}
