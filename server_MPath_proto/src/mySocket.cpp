#include "../include/common.h"
#include "../include/mySocket.h"

#define ON_REUSEADDR  1  //you can reuse the addr after binding addr without no waiting time 
//#define OFF_REUSEADDR 0

Transmitter::~Transmitter() {
	if(sock_id > 0)
		close(sock_id);
}

void Transmitter::Socket_for_udp() {
	if((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Create socket falied\n");
		exit(0);
	}
}

void Transmitter::Setsockopt(int sock_id, int level, int option_name,
               void *option_value, int option_len) {
    if(-1 == setsockopt(sock_id, level, 
    	option_name, option_value, option_len)) {
        perror("setsockopt failed!\n");
        exit(0);
    }
}

void Transmitter::Bind(int sock_id, SA *addr_self, int len) const 
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

void Transmitter::transmitter_new(char *addr_self, char *port_self, 
	                               char *addr_dst, char *port_dst) {
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

int Transmitter::Send_udp(char *data, int len) {
	int num_sent = 0;

	if((num_sent = sendto(sock_id, data, len, 0, 
		(SA *)&(server_addr), sizeof(SA))) < 0) {
		printf("\n!!!send failed, send %d bytes!!!!!!\n", num_sent);
		exit(0);
	}
	return num_sent;
}

int Transmitter::Recv_udp(char *buf_dst, int len) {
	int num_recv = 0;
	socklen_t len_server_addr;

	if ((num_recv = recvfrom(sock_id, buf_dst, len, 0, 
	   (SA *)&(server_addr), &len_server_addr)) < 0) {
		printf("\n!!!recv failed, just recv %d bytes!!!!!!\n", num_recv);
		exit(0);
	}
	return num_recv;
}

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: the thread function which implement data sending procedure 
//Parameter:   num_core   
//			   id_path
//			   argv[]
//             param_encd
//==========================================================================
void Transmitter::send_thread(struct Param_Transmitter param_transmit,
	                              Data_Manager data_manager) {
	Transmitter _client;
	char packet[param_encd.S + LEN_CONTRL_MSG];

	affinity_set(param_transmit.num_core);

	_client.udp_sock_client_new(param_transmit.addr_self, 
		                        param_transmit.port_self, 
		                        param_transmit.addr_dst,
		                        param_transmit.port_dst);

	while(1) {
//fetch the data from send_Q, queue buffer
		data_type *data_tmp = data_manager.send_Q[param_transmit.id_path].front();
		data_manager.send_Q[param_transmit.id_path].pop();

		for(int i=0; i < param_encd.K; i++) {
			packet_encaps(packet, &(data_tmp[i*param_encd.S]), param_encd.S);
			_client.Send_udp(packet, param_encd.S + LEN_CONTRL_MSG);
		}
	}
}	
//==========================================================================