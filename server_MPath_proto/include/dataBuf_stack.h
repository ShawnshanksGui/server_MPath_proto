#include "semaphore.h"

typedef struct node {
	char            *data;
	int 		block_num;
	int 	data_len_node;   //data lenth stored in one node
	struct node     *next;
}node, *p_node;

struct Data_buf {
	p_node     top;
	p_node  bottom;
	int slot_empty;
	int  slot_full;
	sem_t    mutex;  //protects accesses to this stack buffer
//	long int dataBytes_remain;  //the remaining data of sender buffer 
};

void init_Data_buf(struct Data_buf * ps);

int  Is_empty(struct Data_buf *ps);

void push(struct Data_buf *ps, int block_num, int data_len_node);

void pop(struct Data_buf *ps, int *block_num, int *data_len_node);

void traverse(struct Data_buf *ps);

void buf_free(struct Data_buf *p_buf);   //free whole the link list buffer  
