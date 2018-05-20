#include "poll.h"
#include "../include/common.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/socket.h"


#define ON_REUSEADDR  1  //you can reuse the addr after binding addr without no waiting time 
//#define OFF_REUSEADDR 0

#define DATA 1
#define CONTROL 2

#define SEG_STOP 0;

//
#define LISTENQ 100

typedef unsigned char uchar;
typedef struct sockaddr SA;

using namespace std;

int sock_id;
int listen_sock_fd;


struct sockaddr_in client_addr;
struct sockaddr_in server_addr;

socklen_t client_addr_len;
socklen_t server_addr_len;



void 
Socket_for_tcp() {
	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Create socket falied\n");
		exit(0);
	}	
}

void 
Setsockopt(int sock_id, int level, int option_name,
           void *option_value, int option_len) {
    if(-1 == setsockopt(sock_id, level, 
    	option_name, option_value, option_len)) {
        perror("setsockopt failed!\n");
        exit(0);
    }
}

void
Bind(int sock_id, SA *addr_self, int len)
{
	if(-1 == bind(sock_id, addr_self, len)) {
		perror("bind failed!!!");
		exit(0);
	}
}	


int CheckConnect(int iSocket) {
	struct pollfd fd;
	int ret = 0;
	socklen_t len = 0;

	fd.fd = iSocket;
	fd.events = POLLOUT;

	while ( poll (&fd, 1, -1) == -1 ) {
		if( errno != EINTR ){
			perror("poll");
//			return -1;
			exit(0);
		}
	}

	len = sizeof(ret);
	if ( getsockopt (iSocket, SOL_SOCKET, SO_ERROR, &ret, &len) == -1 ) {
    	perror("getsockopt");
//		return -1;
		exit(0);
	}

	if(ret != 0) {
		fprintf (stderr, "socket %d connect failed: %s\n",
                 iSocket, strerror (ret));
//		return -1;
		exit(0);
	}

	return 0;
}

//  tcp needs to establish a connections!!!
void
Connect() {
	if(connect(sock_id, (SA *)&server_addr, sizeof(server_addr)) < 0) {
    	if(errno != EINPROGRESS) { 
	  		perror("Connect socket failed!\n");
        	exit(0);
		}
		else{
			perror("connect");
			printf("check delay connection\n");
			CheckConnect(sock_id);			
		}	
 	}
}


void 
transmitter_new_tcp_sponsor(char *addr_self, char *port_self,
                			char *addr_dst, char *port_dst) {
    memset(&(server_addr), 0, sizeof(server_addr));
    memset(&(client_addr), 0, sizeof(client_addr));

    Socket_for_tcp();

//enable fastly recover the port which just has been occupied. 
    int state_reuseAddr              = ON_REUSEADDR;
    Setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR,
               &state_reuseAddr, sizeof(state_reuseAddr));
//set the size of recv buffer
    int recv_buf_size=1*1024*1024*1024;
    Setsockopt(sock_id, SOL_SOCKET, SO_RCVBUF, (char *)&recv_buf_size, sizeof(int));

    server_addr.sin_family  = AF_INET;
    server_addr.sin_port    = htons(atoi(port_dst));
    inet_pton(AF_INET, addr_dst, &(server_addr.sin_addr));

    client_addr.sin_family = AF_INET;
    client_addr.sin_port   = htons(atoi(port_self));
    inet_pton(AF_INET, addr_self, &(client_addr.sin_addr));

    Bind(sock_id, (struct sockaddr *)&client_addr, sizeof(client_addr));

//  tcp need to establish a connection!!!
  	Connect();  
}	


int main(int argc, char **argv) {
	char pkt_snd[10] = {'\0'};

	transmitter_new_tcp_sponsor(argv[1], argv[2], argv[3], argv[4]);

	send(sock_id, pkt_snd, 10, 0);

//	printf("%s\n", pkt_recv);

	return 0;
}