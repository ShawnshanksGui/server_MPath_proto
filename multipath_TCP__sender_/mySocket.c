#include "common.h"
#include "mySocket.h"

#define ON_REUSEADDR  1  //you can reuse the addr after binding addr without no waiting time 
//#define OFF_REUSEADDR 0

int Socket_for_tcp(int sock_id)
{
	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Create socket falied\n");
		exit(0);
	}

	return sock_id;
}

int Socket_for_udp(int sock_id)
{
	if((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Create socket falied\n");
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

void Bind(int sock_id, SA *addr_self, int len)
{
	if(bind(sock_id, addr_self, len)) {
		perror("bind failed!!!");
		exit(0);
	}
}

void Connect(int sock_id, struct sockaddr *serv_addr, int len_sock_addr){
	if(-1 == connect(sock_id, serv_addr, len_sock_addr)) {
       	perror("Connect socket failed!\n");
        exit(0);
    }
}

void *tcp_sock_sender_new(char *addr_self, char *port_self, char *addr_dst, char *port_dst) {
	struct Socket *_socket = (struct Socket *)malloc(sizeof(struct Socket));

	memset(&(_socket->address_dst) , 0,  sizeof(_socket->address_dst));
	memset(&(_socket->address_self), 0, sizeof(_socket->address_self));
	
	_socket->sock_id                 = Socket_for_tcp(_socket->sock_id);

	int state_reuseAddr              = ON_REUSEADDR;
	Setsockopt(_socket->sock_id, SOL_SOCKET, SO_REUSEADDR, 
		       &state_reuseAddr, sizeof(state_reuseAddr));

	_socket->address_dst.sin_family  = AF_INET;
	_socket->address_dst.sin_port    = htons(atoi(port_dst));
	inet_pton(AF_INET, addr_dst, &(_socket->address_dst.sin_addr));

	_socket->address_self.sin_family = AF_INET;
	_socket->address_self.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(_socket->address_self.sin_addr));
	Bind(_socket->sock_id, (SA *)&(_socket->address_self), sizeof(_socket->address_self));

	Connect(_socket->sock_id, (SA *)&(_socket->address_dst), sizeof(struct sockaddr));

	return _socket;
}

void *udp_sock_sender_new(char *addr_self, char *port_self, char *addr_dst, char *port_dst) {
	struct Socket *_socket = (struct Socket *)malloc(sizeof(struct Socket));

	memset(&(_socket->address_dst), 0, sizeof(_socket->address_dst));
	memset(&(_socket->address_self), 0, sizeof(_socket->address_self));
	
	_socket->sock_id                 = Socket_for_udp(_socket->sock_id);

	int state_reuseAddr              = ON_REUSEADDR;
	Setsockopt(_socket->sock_id, SOL_SOCKET, SO_REUSEADDR, 
		       &state_reuseAddr, sizeof(state_reuseAddr));

	_socket->address_dst.sin_family  = AF_INET;
	_socket->address_dst.sin_port    = htons(atoi(port_dst));
	inet_pton(AF_INET, addr_dst, &(_socket->address_dst.sin_addr));

	_socket->address_self.sin_family = AF_INET;
	_socket->address_self.sin_port   = htons(atoi(port_self));
	inet_pton(AF_INET, addr_self, &(_socket->address_self.sin_addr));
	Bind(_socket->sock_id, (struct sockaddr *)&_socket->address_self, sizeof(_socket->address_self));

	return _socket;
//	Connect(_socket->sock_id, (struct sockaddr *)&_socket->address_dst, sizeof(struct sockaddr));
}

int Send_tcp(struct Socket *_socket, char *data, int len){
	int num_sent = 0;

	if((num_sent = send(_socket->sock_id, data, len, 0)) <= 0) {
		perror("\n!!!send failed!!!!!!");
		exit(0);
	}

	return num_sent;
}

int Send_udp(struct Socket *_socket, char *data, int len){
	int num_sent = 0;

	if((num_sent = sendto(_socket->sock_id, data, len, 0, (SA *)&(_socket->address_dst), sizeof(SA))) <= 0) {
		perror("\n!!!send failed!!!!!!");
		exit(0);
	}

	return num_sent;
}