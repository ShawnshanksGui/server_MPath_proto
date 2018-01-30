#include  "common.h"
#include "_socket.h"

#define ON_REUSEADDR  1  //you can reuse the addr after binding addr without no waiting time 
//#define OFF_REUSEADDR 0

void close_socket(struct Socket *_socket) {
	if(_socket->connect_id > 0)
		close(_socket->connect_id);
	close(_socket->sock_id);
}

int Socket_for_tcp(int sock_id){
	if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Create socket falied\n");
		exit(0);
	}

	return sock_id;
}

int Socket_for_udp(int sock_id) {
	if ((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Create socket falied!\n");
		exit(0);
	}

	return sock_id;
}

void Setsockopt(int sock_id, int level, int option_name, 
		       void *option_value, int option_len) {
	if(-1 == setsockopt(sock_id, level, option_name, option_value, option_len)) {		
		perror("setsockopt failed!\n");
		exit(0);
	}
}

void Bind(int sock_id, SA *addr_self, int len) {
	if (bind(sock_id, addr_self, len)) {
		perror("bind failed!");
		exit(0);
	}
}

void Listen(struct Socket *_socket) {
	if (listen(_socket->sock_id, 1024) != 0) {
        perror("listen error happened!\n");
        exit(0);
    }
}

void Connect(int sock_id, struct sockaddr *serv_addr, int len_sock_addr){
	if (-1 == connect(sock_id, serv_addr, len_sock_addr)) {
       	perror("Connect socket failed!\n");
        exit(0);
    }
}


void *tcp_sock_recver_new(char *addr_self, char *port_self) {
	struct Socket *_socket = (struct Socket *)malloc(sizeof(struct Socket));

	memset(&(_socket->address_self), 0, sizeof(_socket->address_self));
	
	_socket->connect_id              = 0;   //for free socket called as connect_id
	
	_socket->sock_id                 = Socket_for_tcp(_socket->sock_id);

	int state_reuseAddr              = ON_REUSEADDR;
	Setsockopt(_socket->sock_id, SOL_SOCKET, SO_REUSEADDR,
               &state_reuseAddr, sizeof(state_reuseAddr));

	_socket->address_self.sin_family = AF_INET;
	_socket->address_self.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(_socket->address_self.sin_addr));

	Bind(_socket->sock_id, (SA *)&(_socket->address_self), sizeof(_socket->address_self));

	Listen(_socket); 

    while ((_socket->connect_id = accept(_socket->sock_id, 
    	(SA *)&(_socket->address_dst), &_socket->len_address_dst)) <= 0);

	return _socket;
}

void *udp_sock_recver_new(char *addr_self, char *port_self) {
	struct Socket *_socket = (struct Socket *)malloc(sizeof(struct Socket));

	memset(&(_socket->address_self), 0, sizeof(_socket->address_self));

	_socket->connect_id = 0;   //for free socket called as connect_id
	
	_socket->sock_id    = Socket_for_udp(_socket->sock_id);

    int state_reuseAddr = ON_REUSEADDR;
    Setsockopt(_socket->sock_id, SOL_SOCKET, SO_REUSEADDR, 
               &state_reuseAddr, sizeof(state_reuseAddr));

	_socket->address_self.sin_family = AF_INET;
	_socket->address_self.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(_socket->address_self.sin_addr));

	Bind(_socket->sock_id, (SA *)&_socket->address_self, sizeof(_socket->address_self));

	return _socket;
}

int Send_tcp(struct Socket *_socket, char *data, int len) {
	int num_sent = 0;

	if ((num_sent = send(_socket->sock_id, data, len, 0)) <= 0) {
		printf("\n!!!send failed, just send %d bytes!!!!!!\n", num_sent);
		exit(0);
	}

	return num_sent;
}

int Send_udp(struct Socket *_socket, char *data, int len) {
	int num_sent = 0;

	if ((num_sent = sendto(_socket->sock_id, data, len, 0, 
		(SA *)&(_socket->address_dst), sizeof(SA))) <= 0) {
		printf("\n!!!send failed, just send %d bytes!!!!!!\n", num_sent);
		exit(0);
	}

	return num_sent;
}

int Recv_tcp(struct Socket *_socket, char *buf_dst, int len) {
	int num_recv = 0;

	if ((num_recv = recv(_socket->connect_id, buf_dst, len, 0)) <= 0) {
		printf("\n!!!recv failed, just recv %d bytes!!!!!!\n", num_recv);
//		exit(0);
	}

	return num_recv;
}

int Recv_udp(struct Socket *_socket, char *buf_dst, int len) {
	int num_recv = 0;

	if ((num_recv = recvfrom(_socket->sock_id, buf_dst, len, 0, 
	   (SA *)&(_socket->address_dst), &(_socket->len_address_dst))) <= 0){
		printf("\n!!!recv failed, just recv %d bytes!!!!!!\n", num_recv);
		exit(0);
	}

	return num_recv;
}





