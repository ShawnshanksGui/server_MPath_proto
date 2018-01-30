#include  "common.h"
#include "mySocket.h"

#define ON_REUSEADDR  1  //you can reuse the addr after binding addr without no waiting time 
//#define OFF_REUSEADDR 0

Udp_sock::~Udp_sock() {
	close(sock_id);
}

void Udp_sock::Socket_for_udp() {
	if ((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Create socket falied!\n");
		exit(0);
	}
}

void Udp_sock::Setsockopt(int sock_id, int level, int option_name, 
		       void *option_value, int option_len) {
	if(-1 == setsockopt(sock_id, level, option_name, option_value, option_len)) {		
		perror("setsockopt failed!\n");
		exit(0);
	}
}

void Udp_sock::Bind(int sock_id, SA *addr_self, int len) const{
	if (bind(sock_id, addr_self, len)) {
		perror("bind failed!");
		exit(0);
	}
}

void Udp_sock::Listen(int scok_id) {
	if (listen(sock_id, 1024) != 0) {
        perror("listen error happened!\n");
        exit(0);
    }
}

void Udp_sock::udp_sock_server_new(char *addr_self, char *port_self) {
//	struct Socket *_socket = (struct Socket *)malloc(sizeof(struct Socket));

	memset(&(server_addr), 0, sizeof(server_addr));
	
	Socket_for_udp();

    int state_reuseAddr = ON_REUSEADDR;
    Setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, 
               &state_reuseAddr, sizeof(state_reuseAddr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(server_addr.sin_addr));

	Bind(sock_id, (SA *)(&server_addr), sizeof(server_addr));
}

int Udp_sock::Send_udp(char *data, int len) {
	int num_sent = 0;

	if ((num_sent = sendto(sock_id, data, len, 0, 
		(SA *)&(client_addr), sizeof(SA))) < 0) {
		printf("\n!!!send failed, just send %d bytes!!!!!!\n", num_sent);
		exit(0);
	}
	return num_sent;
}

int Udp_sock::Recv_udp(char *buf_dst, int len) {
	int num_recv = 0;
	socklen_t len_address_client;

	if ((num_recv = recvfrom(sock_id, buf_dst, len, 0, 
	   (SA *)&(client_addr), &(len_address_client))) < 0){
		printf("\n!!!recv failed, just recv %d bytes!!!!!!\n", num_recv);
		exit(0);
	}
	return num_recv;
}





