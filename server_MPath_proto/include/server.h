#ifndef _SERVER_H
#define _SERVER_H

class Server {
	queue<char *> send_Que;



	void Data_Manager::sender_thread_a(){
	
		while(1) {
			char *data = encd_Q[id_path].front(); 
			encd_Q[id_path].pop();	
			for(int i = 0; i < ENCD_BLOCK_SIZE; i++) {
				Udp_send(encd_block[i*SYMBOL_SIZE], SYMBOL_SIZE);
			}
		}	
	}
}; 

#endif