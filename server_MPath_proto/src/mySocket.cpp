#include "../include/thread_core_affinity_set.h"

#include "../include/encoder.h"

#include "../include/common.h"
#include "../include/mySocket.h"
#include "../include/data_manager.h"

#define ON_REUSEADDR  1  //you can reuse the addr after binding addr without no waiting time 
//#define OFF_REUSEADDR 0

#define DATA 1
#define CONTROL 2

//
#define LISTENQ 100

typedef unsigned char uchar;

Transmitter::
~Transmitter() {
	if(sock_id > 0) {
		close(sock_id);
	}
	if(listen_sock_fd > 0) {
		close(listen_sock_fd);
	}
}

void Transmitter::
Socket_for_udp() {
	if((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("\nCreate udp socket falied\n");
		exit(0);
	}
}

void Transmitter::
Socket_for_tcp_listen() {
	if((listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("\nCreate tcp listen socket falied\n");
		exit(0);
	}
}

void Transmitter::
Setsockopt(int sock, int level, int option_name,
               void *option_value, int option_len) {
    if(-1 == setsockopt(sock, level, 
    	option_name, option_value, option_len)) {
        perror("setsockopt failed!\n");
        exit(0);
    }
}

void Transmitter::
Bind(int sock, SA *addr_self, int len) const 
{
	if(bind(sock, addr_self, len)) {
		perror("bind failed!!!");
		exit(0);
	}
}


void Transmitter::
Listen() {
	if(listen(listen_sock_fd, LISTENQ) == -1) {
		perror("\nlisten error happened!\n");
		exit(0);
	}
}

void Transmitter::
Accept() {
	if((sock_id = accept(listen_sock_fd, (SA *)&client_addr,
						 &client_addr_len)) < 0) {
		perror("\nAccept failed!\n");
		exit(0);
	}
}


void Transmitter::
transmitter_new_udp(char *addr_self, char *port_self, 
	                 char *addr_dst,  char *port_dst) {
	memset(&(server_addr), 0, sizeof(server_addr));
	memset(&(client_addr), 0, sizeof(client_addr));
	
	Socket_for_udp();

//enable fastly recover the port which just has been occupied. 
	int state_reuseAddr              = ON_REUSEADDR;
	Setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, 
		       &state_reuseAddr, sizeof(state_reuseAddr));

//set the size of recv buffer
	int recv_buf_size=1*1024*1024*1024;
	Setsockopt(sock_id, SOL_SOCKET, SO_RCVBUF, (char *)&recv_buf_size, sizeof(int));

	client_addr.sin_family  = AF_INET;
	client_addr.sin_port    = htons(atoi(port_dst));
	inet_pton(AF_INET, addr_dst, &(client_addr.sin_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(server_addr.sin_addr));

	Bind(sock_id, (SA *)&server_addr, sizeof(server_addr));
//  udp needn't establish any connection!!!
//	Connect(sock_id, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));  
}


void Transmitter::
transmitter_new_tcp(char *addr_self, char *port_self) {
	memset(&(server_addr), 0, sizeof(server_addr));
//	memset(&(client_addr), 0, sizeof(client_addr));

	Socket_for_tcp_listen();

//enable fastly recover the port which just has been occupied. 
	int state_reuseAddr = ON_REUSEADDR;
	Setsockopt(listen_sock_fd, SOL_SOCKET, SO_REUSEADDR, 
		       &state_reuseAddr, sizeof(state_reuseAddr));
//set the size of recv buffer
	int recv_buf_size = 1*1024*1024*1024;
	Setsockopt(listen_sock_fd, SOL_SOCKET, SO_RCVBUF, 
			   (char *)&recv_buf_size, sizeof(int));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(server_addr.sin_addr));

	Bind(listen_sock_fd, (SA *)&server_addr, sizeof(server_addr));

	Listen();
	Accept();
}


int Transmitter::
Send_udp(char *data_src, int len) {
	int num_sent = 0;

	if((num_sent = sendto(sock_id, data_src, len, 0, 
		(SA *)&(client_addr), sizeof(SA))) < 0) {
		printf("\n!!!send failed, send %d bytes!!!!!!\n", num_sent);
		exit(0);
	}
	return num_sent;
}

int Transmitter::
Recv_udp(char *data_dst, int len) {
	int num_recv = 0;
	socklen_t len_server_addr;

	if ((num_recv = recvfrom(sock_id, data_dst, len, 0, 
	   (SA *)&(client_addr), &len_server_addr)) < 0) {
		printf("\n!!!recv failed, just recv %d bytes!!!!!!\n", num_recv);
		exit(0);
	}
	return num_recv;
}


int Transmitter::
Send_tcp(char *data, int len) {
	int len_sent = 0;
	len_sent = sendto(sock_id, data, len, 0, (SA *)&client_addr,
					  client_addr_len);
	if(-1 == len_sent) {
		perror("sendto failed");
		exit(0);
	}
	else if(len_sent != len) {
		printf("\nOnly send %dbytes data\n", len_sent);
	}
	return len_sent;
}

int Transmitter::
Recv_tcp(char *data_dst, int len) {
	int len_recv;
	len_recv = recvfrom(sock_id, data_dst, len, 0, (SA *)&client_addr, 
						&client_addr_len);
	if(-1 == len_recv) {
		perror("\nrecv failed\n");
		exit(0);
	}
	else if(len_recv != len) {
		printf("\nOnly recv %dbytes data\n", len_recv);
		exit(0);
	}
	return len_recv;
}

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:  
//Description: the thread function which implement data sending procedure 
//Parameter:   num_core   
//			   id_path
//			   argv[]
//             param_encd
//==========================================================================
void Transmitter::
send_td_func(int id_path, Data_Manager &data_manager) {
	Encoder encoder;
	VData_Type packet[1000 + LEN_CONTRL_MSG];

	affinity_set(id_path);
//
	int cnt_packet = 0;

	int cnt_block;
	int prev_id_seg = 100000;

	VData_Type *data_tmp = nullptr;
	VData_Type *scapegoat = nullptr;
	encoder.encoder_init();

	while(1) {
//fetch the data from send_Q, queue buffer.
		shared_ptr<struct Elem_Data> data_elem;
		while(nullptr == (data_elem = data_manager.data_fetch(id_path)));

		if(data_elem->id_seg != prev_id_seg) {
			cnt_block = 0;
			prev_id_seg = data_elem->id_seg;
		}
		if(data_elem->size < (data_elem->S_FEC*data_elem->K_FEC)) {
			scapegoat = MALLOC(VData_Type, data_elem->S_FEC*data_elem->K_FEC);
			memcpy(scapegoat, data_elem->data, data_elem->size);
//zero padding for the block, which length is short than S*K
			memset(&(scapegoat[data_elem->size]), 0, \
				   data_elem->S_FEC*data_elem->K_FEC - data_elem->size);
			data_tmp = (VData_Type *)encoder.encode(scapegoat, \
							  				 data_elem->S_FEC, data_elem->K_FEC, \
											 data_elem->M_FEC);
			SAFE_FREE(scapegoat);
		}
		else if((data_elem->S_FEC*data_elem->K_FEC) == data_elem->size){
			data_tmp = (VData_Type *)encoder.encode(data_elem->data, \
									  		 data_elem->S_FEC, data_elem->K_FEC, \
										     data_elem->M_FEC);
		}
		else {
			printf("\nerror happened in fetching data_elem!\n");
			printf("this real data size = %d, the S*K = %d\n", 
					data_elem->size, data_elem->S_FEC*data_elem->K_FEC);
			exit(0);
		}
//		data_manager.data_video[id_path].pop();
		for(int i = 0; i < data_elem->K_FEC; i++) {
			if(data_elem->S_FEC != SYMBOL_FEC) {printf("the S of data_elem error!\n");}
			memset(packet, 0, data_elem->S_FEC + LEN_CONTRL_MSG);
			encaps_packet(packet, cnt_block, i, \
						  &(data_tmp[i*data_elem->S_FEC]), data_elem);

			int num_sent = Send_udp(packet, data_elem->S_FEC + LEN_CONTRL_MSG);
//			printf("sent a pkt with %d bytes\n", num_sent);
			printf("This is the %d-th  packet\n", ++cnt_packet);
		}
		usleep(100);
		SAFE_FREE(data_tmp);

		cnt_block++;
	}
}

void Transmitter::
encaps_packet(VData_Type *packet, int block_id, int symbol_id, 
			  VData_Type *data_src, shared_ptr <struct Elem_Data> data_elem) {
	packet[0] = (uchar)DATA;
	packet[1] = (uchar)data_elem->id_seg;
	packet[2] = (uchar)data_elem->id_region;
 	*((int *)&packet[3]) = data_elem->size;
//specify id_symbol. 
	packet[7] = (uchar)block_id;
	packet[8] = (uchar)symbol_id;
//0 denotes the default level.
	packet[9]  = (uchar)0;
	packet[10] = (uchar)data_elem->K_FEC;
	packet[11] = (uchar)data_elem->M_FEC;	

	memcpy(&(packet[LEN_CONTRL_MSG]), data_src, data_elem->S_FEC);
	if(0 == symbol_id) {	
		printf("This is the imposed information, as following:\n");
        printf("id_seg = %d, id_region = %d, originBlk_size = %d, block_id = %d, symbol_id = %d, s_level = %d, k_fec = %d, m_fec = %d\n", \
        		data_elem->id_seg, data_elem->id_region, data_elem->size, block_id, symbol_id, 0, data_elem->K_FEC, data_elem->M_FEC);

 		printf("And this is the packet info, as following:\n");
        printf("id_seg = %d, id_region = %d, originBlk_size = %d, block_id = %d, symbol_id = %d, s_level = %d, k_fec = %d, m_fec = %d\n", \
        		(uchar)packet[1],(uchar)packet[2],*(int *)(&packet[3]),(uchar)packet[7],(uchar)packet[8],(uchar)packet[9],(uchar)packet[10],(uchar)packet[11]);
	}
}
//==========================================================================