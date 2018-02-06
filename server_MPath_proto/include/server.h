//#define TCP                    6
//#define UDP                   17

//control procedure sending or stopping
#define STOP                  0
#define READY                 1

//the amount of command parameters required 
#define NUM_PARA              6

//finish reading for a video file  
#define END_FILE              0

#define MAX_BUF               10 

#define TEST_SECONDS         0.1

#define TOTAL_WORK_THREADS    10

#define CODELINE             100
#define SYMB_SIZE          100
//original data block size
#define ORIGIN_BLOCK_SIZE    240  
#define ENCD_BLOCK_SIZE      256    

/*
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
*/