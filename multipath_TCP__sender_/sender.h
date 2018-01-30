#define CODELINE             1000
#define MAX_INPUT_SYMBOL_NUM 20

#define TOTAL_WORK_THREADS 1

#define TEST_SECONDS      5

#define MAX_BUF 10 

#define TCP  6
#define UDP 17

//control procedure sending or stopping
#define READY 1
#define STOP  0


struct Transmit{
	int type_proto;
	void* (* socket_new)(char *addr_self, char *port_self, char *addr_dst, char *port_dst);
	int (* _send)(struct Socket *_socket, char *data, int len);

};

struct Args{
	char id_core;

	char *type_proto_str;
	char  *addr_self_str;
	char  *port_self_str;
	char   *addr_dst_str;
	char   *port_dst_str;
};

void *transmit_new(char *);
