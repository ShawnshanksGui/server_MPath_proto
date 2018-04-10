#ifndef _MYSOCKET_H_
#define _MYSOCKET_H_

#include "fcntl.h"
#include "sys/types.h"
#include "sys/socket.h"

#include "data_manager.h"


typedef struct sockaddr SA;

class Transmitter {
private:
	int sock_id;
	int listen_sock_fd;

	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;

	socklen_t client_addr_len;
	socklen_t server_addr_len;

	void Socket_for_udp();
	void Socket_for_tcp_listen();

	void Setsockopt(int , int , int, void *, int);

	void Bind(int, SA *, int) const;

	void Listen();

	void Accept();

//  Udp socket doesn't need to establish any connections!!!
//	void Connect(int sock_id, struct sockaddr *serv_addr, int len_sock_addr) const;

public:
//	Transmitter(int, struct sockaddr_in, struct sockaddr_in);
	Transmitter() {
		client_addr_len = sizeof(client_addr);
		server_addr_len = sizeof(server_addr); 
	}

	~Transmitter();

	void transmitter_new_udp(char *addr_self, char *port_self, 
		                 char *addr_dst,  char *port_dst); 

	void transmitter_new_tcp(char *add_self, char *port_self);
	
	void send_td_func(int id_path, Data_Manager &data_manager);

	void encaps_packet(VData_Type *packet, int block_id, int num,
					   VData_Type *data_src, shared_ptr <struct Elem_Data> data_elem);

	int Send_udp(char *data, int len);
	int Recv_udp(char *buf_dst, int len);

	int Send_tcp(char *data, int len);
	int Recv_tcp(char *data_dst, int len);
};

/*
struct Param_Transmitter{
	int num_core;
	int id_path;
	char *addr_self;
	char *port_self; 
	char *addr_dst; 
	char *port_dst;	
};
*/
/*
typedef struct sockaddr SA;

class Transmitter {
private:
	int sock_id;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;

	void Socket_for_udp();

	void Setsockopt(int , int , int, void *, int);

	void Bind(int, SA *, int) const;

//  Udp socket doesn't need to establish any connections!!!
//	void Connect(int sock_id, struct sockaddr *serv_addr, int len_sock_addr) const;

public:
//	Transmitter(int, struct sockaddr_in, struct sockaddr_in);
	Transmitter() {}

	~Transmitter();

	void transmitter_new(char *addr_self, char *port_self, 
		                 char *addr_dst,  char *port_dst); 
	
	void send_td_func(int id_path, Data_Manager &data_manager);
	void encaps_packet(VData_Type *packet, int block_id, int num,
					   VData_Type *data_src, shared_ptr <struct Elem_Data> data_elem);

	int Send_udp(char *data, int len);
	int Recv_udp(char *buf_dst, int len);
};
*/
#endif