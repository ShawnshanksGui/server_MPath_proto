#ifndef _MYSOCKET_H_
#define _MYSOCKET_H_

#include "fcntl.h"
#include "sys/types.h"
#include "sys/socket.h"

class Udp_sock {
private:
	int sock_id;
	struct sockaddr_in client_addr;  
	struct sockaddr_in server_addr;  //self

	void Socket_for_udp();

	void Setsockopt(int, int , int, void *, int);

	void Listen(int); 

	void Bind(int, SA *, int) const;

//  udp needn't establish any connection!!!
//	void Connect(int sock_id, struct sockaddr *serv_addr, int len_sock_addr) const;

public:
//	Udp_sock(int, struct sockaddr_in, struct sockaddr_in);
	Udp_sock() {}

	~Udp_sock();

	void udp_sock_server_new(char *addr_self, char *port_self); 
	
	int Send_udp(char *data, int len);
	int Recv_udp(char *buf_dst, int len);
};

#endif