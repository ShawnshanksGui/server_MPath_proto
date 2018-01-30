#include "fcntl.h"
#include "sys/types.h"
#include "sys/socket.h"

struct Socket{
	int sock_id;
	struct sockaddr_in address_self;
	struct sockaddr_in  address_dst;
};

int Socket_for_tcp(int sock_id);
int Socket_for_udp(int sock_id);

void Setsockopt(int , int , int, void *, int);

void Bind(int, SA *, int);

void  Connect(int sock_id, struct sockaddr *serv_addr, int len_sock_addr);

void *tcp_sock_sender_new(char *addr_self, char *port_self, char *addr_dst, char *port_dst);
void *udp_sock_sender_new(char *addr_self, char *port_self, char *addr_dst, char *port_dst); 

int Send_tcp(struct Socket *_socket, char *data, int len);
int Send_udp(struct Socket *_socket, char *data, int len);






