#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <queue>

#include <thread>
#include <mutex>
#include "system_params.h"

typedef int     ID_BUF;
typedef int    ID_PATH;
typedef char VData_Type;

//Judge whether PUSH is succesfully or not! 
#define FAIL_PUSH 0
#define SUCS_PUSH 1

//specify all the ID number of core bound to responding thread
#define DATA_GEN_CORE      0
#define DATA_HANDLER_CORE  1
#define TRANSMIT_CORE      2

//the bytes of control message bytes  in one packet 
#define LEN_CONTRL_MSG 10


using namespace std;

struct Param_Reader {
	char *path_input;
//	int len_block; 
};


class Data_Manager {
public:	
	queue<char *> data_video[NUM_PATH];
	queue<char *> send_Q[NUM_PATH];

	Data_Manager(int max_size);
	Data_Manager();
	~Data_Manager();

//	void path_decision(int &id_path, double plr[NUM_PATH], 
//		               double RTT[NUM_PATH]);
//	void para_FEC_decision(struct Para_encd &para_encd, double plr);

//	void video_reader_thread();
	void data_handler_thread();

//friend classes
	friend class Video_Reader;
	friend class Encoder;
	friend class Transmitter;

private:
	std:mutex mtx[2];
	
	int buf_size[NUM_PATH];
	int MAX_SIZE[NUM_PATH];

	bool data_save(data_type *data, ID_PATH id_path);
	data_type *data_fetch(ID_PATH id_path);

	bool Is_empty(ID_BUF id_buf);
	bool Is_overflow(ID_BUF id_buf);
	bool Push(data_type *data_src, ID_PATH id_path);
	data_type *Pop(ID_PATH id_path);

//video reader
//	void gen_param_reader(char input_video_path);

//transmitter
	void Init_param_transmitter(struct Param_Transmitter &param_transmit);
};

#endif