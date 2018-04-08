#include "../include/thread_core_affinity_set.h"

#include "../include/encoder.h"

#include "../include/common.h"
#include "../include/mySocket.h"
#include "../include/data_manager.h"

#define ON_REUSEADDR  1  //you can reuse the addr after binding addr without no waiting time 
//#define OFF_REUSEADDR 0

#define DATA 1
#define CONTROL 2

typedef unsigned char uchar;

Transmitter::
~Transmitter() {
	if(sock_id > 0)
		close(sock_id);
}

void Transmitter::
Socket_for_udp() {
	if((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Create socket falied\n");
		exit(0);
	}
}

void Transmitter::
Setsockopt(int sock_id, int level, int option_name,
               void *option_value, int option_len) {
    if(-1 == setsockopt(sock_id, level, 
    	option_name, option_value, option_len)) {
        perror("setsockopt failed!\n");
        exit(0);
    }
}

void Transmitter::
Bind(int sock_id, SA *addr_self, int len) const 
{
	if(bind(sock_id, addr_self, len)) {
		perror("bind failed!!!");
		exit(0);
	}
}

//  udp protocol doesn't neede to establish any connections!!!

//void Transmitter::Connect(int sock_id, struct sockaddr *serv_addr, int len_sock_addr) const {
//	if(-1 == connect(sock_id, serv_addr, len_sock_addr)) {
//       	perror("Connect socket failed!\n");
//        exit(0);
//  }
//}
//
/*
void Transmitter::
	 transmitter_new(char *addr_self, char *port_self, 
	                 char *addr_dst,  char *port_dst) {
	memset(&(server_addr), 0, sizeof(server_addr));
	memset(&(client_addr), 0, sizeof(client_addr));
	
	Socket_for_udp();

//enable fastly recover the port which have being used. 
	int state_reuseAddr              = ON_REUSEADDR;
//	
	Setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, 
		       &state_reuseAddr, sizeof(state_reuseAddr));

	server_addr.sin_family  = AF_INET;
	server_addr.sin_port    = htons(atoi(port_dst));
	inet_pton(AF_INET, addr_dst, &(server_addr.sin_addr));

	client_addr.sin_family = AF_INET;
	client_addr.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(client_addr.sin_addr));

	Bind(sock_id, (struct sockaddr *)&client_addr, sizeof(client_addr));
//  udp needn't establish any connection!!!
//	Connect(sock_id, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));  
}
*/
void Transmitter::
transmitter_new(char *addr_self, char *port_self, 
	                 char *addr_dst,  char *port_dst) {
	memset(&(server_addr), 0, sizeof(server_addr));
	memset(&(client_addr), 0, sizeof(client_addr));
	
	Socket_for_udp();

//enable fastly recover the port which have being used. 
	int state_reuseAddr              = ON_REUSEADDR;
//	
	Setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, 
		       &state_reuseAddr, sizeof(state_reuseAddr));

	client_addr.sin_family  = AF_INET;
	client_addr.sin_port    = htons(atoi(port_dst));
	inet_pton(AF_INET, addr_dst, &(client_addr.sin_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(server_addr.sin_addr));

	Bind(sock_id, (struct sockaddr *)&server_addr, sizeof(server_addr));
//  udp needn't establish any connection!!!
//	Connect(sock_id, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));  
}


int Transmitter::
Send_udp(char *data, int len) {
	int num_sent = 0;

	if((num_sent = sendto(sock_id, data, len, 0, 
		(SA *)&(client_addr), sizeof(SA))) < 0) {
		printf("\n!!!send failed, send %d bytes!!!!!!\n", num_sent);
		exit(0);
	}
	return num_sent;
}

int Transmitter::
Recv_udp(char *buf_dst, int len) {
	int num_recv = 0;
	socklen_t len_server_addr;

	if ((num_recv = recvfrom(sock_id, buf_dst, len, 0, 
	   (SA *)&(client_addr), &len_server_addr)) < 0) {
		printf("\n!!!recv failed, just recv %d bytes!!!!!!\n", num_recv);
		exit(0);
	}
	return num_recv;
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
//fetch the data from send_Q, queue buffer
		memset(packet, 0, 1000 + LEN_CONTRL_MSG);

		shared_ptr<struct Elem_Data> data_elem;

		while(nullptr == (data_elem = data_manager.data_fetch(id_path)));

		if(data_elem->id_seg != prev_id_seg) {
			cnt_block = 0;
			prev_id_seg = data_elem->id_seg;
		}
		if((data_elem->S_FEC*data_elem->K_FEC) != data_elem->size) {
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
		else {
			data_tmp = (VData_Type *)encoder.encode(data_elem->data, \
									  		 data_elem->S_FEC, data_elem->K_FEC, \
										     data_elem->M_FEC);
		}
//		data_manager.data_video[id_path].pop();
		for(int i = 0; i < data_elem->K_FEC; i++) {
			encaps_packet(packet, cnt_block, i, \
						  &(data_tmp[i*data_elem->S_FEC]), data_elem);

			this->Send_udp(packet, data_elem->S_FEC + LEN_CONTRL_MSG);
//			printf("This is the %d-th  packet\n", ++cnt_packet);
		}
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

	memcpy(&(packet[6]), data_src, data_elem->S_FEC);
	if(0 == symbol_id) {
		printf("\nthis is the imposed information, as following:\n");
        printf("id_seg = %d, id_region = %d, originBlk_size = %d, block_id = %d, symbol_id = %d, s_level = %d, k_fec = %d, m_fec = %d\n", \
        		DATA, data_elem->id_seg, data_elem->id_region, block_id, symbol_id, 0, data_elem->K_FEC, data_elem->M_FEC);

 		printf("\nthis is the packet info, as following:\n");
        printf("id_seg = %d, id_region = %d, originBlk_size = %d, block_id = %d, symbol_id = %d, s_level = %d, k_fec = %d, m_fec = %d\n", \
        		packet[1], packet[2], packet[3], packet[7], packet[8], packet[9], packet[10], packet[11]);

	}
				
}
//==========================================================================