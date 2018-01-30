#include "thread_core_affinity_set.h"
#include    "sender.h"
#include    "common.h"
#include   "_socket.h"
#include   "utility.h"
#include   "pthread.h"
#include "semaphore.h"
#include "mySemaphore.h"

void *transmit_new(char *_type)
{
	struct Transmit* _transmit = (struct Transmit *)malloc(sizeof(struct Transmit));
	
	if(!strcmp(_type, "UDP")) {
		_transmit->type_proto = UDP;
		_transmit->socket_new = udp_sock_sender_new;
		_transmit->_send      = Send_udp;
	}
	else if(!strcmp(_type, "TCP")) {
		_transmit->type_proto = TCP;
		_transmit->socket_new = tcp_sock_sender_new;
		_transmit->_send      = Send_tcp;
	}
	else {
		perror("can't match type of transport protcol!");
		exit(0);
	}
	return _transmit; 
}


int thread_control[TOTAL_WORK_THREADS] = {READY};

struct Data_buf send_buf; //
//struct Data_buf send_buf_b;

//semaphore
//sem_t mutex;

void *thread_data_gen(void *args_d) {
	P(&send_buf->slot_empty);
	P(&send_buf->mutex);


}

void *thread_send(void *args_work);

int main(int argc, char *argv[]) {
	int       loct_command = 2;   // sign the location of command!
	pthread_t threads[TOTAL_WORK_THREADS + 1]; 

	Usage(argc, 11, argv[0]);

	init_stack(&send_buf);
 
	Sem_init(&send_buf.mutex, 0, 1);  //protects accesses to this stack buffer
	Sem_init(&send_buf.slot_empty, 0, MAX_BUF);
	Sem_init(&send_buf.slot_full, 0, 0);

//create dataGeneration thread
	pthread_create(&threads[TOTAL_WORK_THREADS], NULL, thread_data_gen, NULL); 
//create packet sending thread
    struct Args *args_work = (struct Args *)malloc(TOTAL_WORK_THREADS * sizeof(struct Args));
	for (int i = 0; i < TOTAL_WORK_THREADS; i ++) {
		args_work[i].id_core        = i;
		args_work[i].type_proto_str = argv[1];
		args_work[i].addr_self_str  = argv[loct_command++];
		args_work[i].port_self_str  = argv[loct_command++];
		args_work[i].addr_dst_str   = argv[loct_command++];
		args_work[i].port_dst_str   = argv[loct_command++];
		pthread_create(&threads[i], NULL, thread_send, (void *)&args_work[i]);
	} 

    usleep(TEST_SECONDS * 1000000);	

    for (int i = 0; i < TOTAL_WORK_THREADS; i++) {
    	thread_control[i] = STOP;       //set terminal ignal for thread
    }
    for (int i = 0; i <= TOTAL_WORK_THREADS; i++) {
        pthread_join(threads[i], NULL); //recycle all threads
    }    

	free(args_work);

	return 0;
}


void *thread_data_gen(void *args_d) {
	while(READY == thread_control[0]) {
		P(&send_buf->slot_empty);
		P(&send_buf->mutex);
//prodeucer
		
//
		V(&send_buf->mutex);
		V(&send_buf->slot_full);
	}
}

void *thread_send(void *args_work) {
	struct timeval starttime;
	struct timeval   endtime;
	double pasttime =  0.0;

	int  len        =    0;
	char *data_send = NULL;
	int  num_sent   =    0;
	char   ctp_buf[CODELINE] = {'\0'};

//pass the type of transport protocol
	struct Args *args = (struct Args *)args_work;
    struct Transmit *_transmit = transmit_new(args->type_proto_str); 
    struct Socket *_socket = _transmit->socket_new(args->addr_self_str,
                                                   args->port_self_str, 
                                                   args->addr_dst_str,
                                                   args->port_dst_str);  
//set affinity between threads and cores 
	affinity_set(args->id_core);

	gettimeofday(&starttime, NULL);

	while(READY == thread_control[0]) {
		P(&send_buf->slot_full);
		P(&send_buf->mutex);
//consummer		
		len = pop(&send_buf, data_send);   
		_transmit->_send(_socket, data_send, len);
		num_sent++;
//
		V(&send_buf->mutex);
		V(&send_buf->slot_empty);
	}

	gettimeofday(&endtime, NULL);
	pasttime = 1000000.0*(endtime.tv_sec - starttime.tv_sec)+(endtime.tv_usec - starttime.tv_usec);
	printf("the data of already sent %lf Mbyte, the time of cost is %lf us\n, sending rate is %lf Mb/s", 
		   (double)num_sent*CODELINE/1000000.0, pasttime, (double)num_sent*CODELINE/pasttime);
	printf("\nsend successfully, the amount of packet is %d\n", num_sent);

//	fclose(fp);
	close(_socket->sock_id);
	free(_transmit);
	free(_socket);
//	exit(0);
}


/*
int main(int argc, char **argv) {
	struct timeval starttime;
	struct timeval   endtime;
	double pasttime =  0.0;

	int    num_sent =    0;

	char ctp_buf[CODELINE] = {'\0'};

	FILE *fp        = NULL;

	Usage(argc, 7, argv[0]);
	Fopen(&fp, argv[6]);

	if((fread(ctp_buf, sizeof(char), CODELINE, fp)) <= 0) {
		perror("fread failed!!!\n");
		exit(0);
	}

    struct Transmit *_transmit = transmit_new(argv[1]);     //argv[1] ---> type of transport protocol

    struct Socket *_socket = _transmit->socket_new(argv[2], argv[3], argv[4], argv[5]);  

	gettimeofday(&starttime, NULL);

	for(int i = 0; i < MAX_INPUT_SYMBOL_NUM; i++) {
		_transmit->_send(_socket, ctp_buf, CODELINE);
	}

	gettimeofday(&endtime, NULL);
	pasttime = 1000000.0*(endtime.tv_sec - starttime.tv_sec)+(endtime.tv_usec - starttime.tv_usec);
	printf("the time of cost is %lf us\n", pasttime);
	printf("\nsend successfully, the amount of packet is %d\n", MAX_INPUT_SYMBOL_NUM);

	fclose(fp);
	close(_socket->sock_id);
	free(_transmit);
	free(_socket);

	exit(0);
}
*/