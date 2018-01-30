#define TCP  6
#define UDP 17

#define RECLINE    1000  // 500
//#define AMOUNT_MAX   160000//?10MB   // 8192// 8192     // 1MB
#define AMOUNT_MAX 20

#define TOTAL_WORK_THREADS 1

struct Transmit{
	int type_proto;

	void* (*socket_new)(char *addr_self, char *port_self);
//	int   (*_send)(struct Socket *_socket, char *data, int len);
	int   (*_recv)(struct Socket *_socket, char *data, int len);
};

struct Args{
	char id_core;

	char *type_proto_str;
	char  *addr_self_str;
	char  *port_self_str;
};

void *transmit_new(char *);


