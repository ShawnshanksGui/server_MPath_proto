#include "common.h"

#include "dataBuf_stack.h"

#define YES 1
#define NO  0

void init_stack(struct Data_buf *p_buf)
{
	if ((p_buf->top = (p_node)malloc(sizeof(node))) == NULL) {
		printf("malloc failed!!!\n");
		exit(0);
	}
	p_buf->bottom     = p_buf->top;
	p_buf->top->next  =       NULL;
//	p_buf->mutex      =         1;    //protect accesses to this stack buf
//	p_buf->slot_empty =    MAX_BUF;
//	p_buf_>slot_full  =         0;

} 

int Is_empty(struct Data_buf *p_buf)
{
	if(p_buf->top == p_buf->bottom)
		return YES;
	else
		return NO;
}

void push(struct Data_buf *p_buf, char *data, int block_num, int data_len_node) {
	p_node p_new = NULL;

	if ((p_new = (p_node)malloc(sizeof(node))) == NULL) {
		perror("push: p_new malloc failed!\n");
		exit(0);
	}
	if(0 == p_buf->slot_empty) {
		printf("the buffer is overflow!\n");
		return;
	}
	p_new->data          = data;
	p_new->block_num     = block_num;
	p_new->data_len_node = data_len_node;
	p_new->next          = p_buf->top;

	p_buf->slot_empty--;
	p_buf->slot_full ++;
	p_buf->top  = p_new;
}

int pop(struct Data_buf *p_buf, char *data_dst) {
	int data_len_node;
	p_node tmp = p_buf->top;
	if (Is_empty(p_buf)) {
		printf("stack is empty!!!\n");
		exit(0);
	}
	else {
//		*block_num = p_buf->top->block_num;
		strcpy(tmp->data, data_dst);
		data_len_node = p_buf->top->data_len_node;

		p_buf->slot_empty++;
		p_buf->slot_full --;
		p_buf->top = p_buf->top->next;

		free(tmp);
	}
	return data_len_node;
}

void traverse(struct Data_buf *p_buf) {
	p_node node_tmp = p_buf->top;
	while(node_tmp != p_buf->bottom) {
		printf("block_num = %d, data_len_node = %d\n", 
			    node_tmp->block_num, node_tmp->data_len_node);
		node_tmp = node_tmp->next;
	}
}

void buf_free(struct Data_buf *p_buf) {
	p_node node_tmp = NULL;

	if (Is_empty(p_buf)) {
		printf("data buffer is empty!!!\n");
	}
	else {
		node_tmp = p_buf->top;
		while(node_tmp != p_buf->bottom) {
			free(node_tmp);
			node_tmp = node_tmp->next;
		}
//		free(p_buf);
	}
}
/*
int main() {
	struct Data_buf block_order_procs;

	int     block_num = 0;
	int data_len_node = 0;

	init_stack(&block_order_procs);
//	while(1) {
		scanf("%d %d\n", &block_num, &data_len_node);
		push(&block_order_procs, block_num, data_len_node);
		traverse(&block_order_procs);
//	}
	
	buf_free(&block_order_procs);	
	return 0;
}
*/