#include "fcntl.h"
#include "sys/types.h"
#include "sys/socket.h"

struct Socket{
	int sock_id;
	int connect_id;    //just for UDP of receiver
	int len_address_dst;
	struct sockaddr_in address_self;
	struct sockaddr_in  address_dst;
};

void close_socket(struct Socket *_socket);

void Setsockopt(int , int , int, void *, int);

void Bind(int, SA *, int);

void Listen(struct Socket *_socket);

void Connect(int sock_id, struct sockaddr *serv_addr, int len_sock_addr);

int Socket_for_tcp(int sock_id);
int Socket_for_udp(int sock_id);

void *tcp_sock_recver_new(char *addr_self, char *port_self);
void *udp_sock_recver_new(char *addr_self, char *port_self); 

int Send_tcp(struct Socket *_socket, char *data, int len);
int Send_udp(struct Socket *_socket, char *data, int len);

int Recv_tcp(struct Socket *_socket, char *buf_recv, int len);
int Recv_udp(struct Socket *_socket, char *buf_recv, int len);

