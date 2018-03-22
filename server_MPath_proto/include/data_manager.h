#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <queue>
#include <mutex>
#include <thread>
#include <vector>

#include "system_params.h"

typedef int      ID_BUF;
typedef int     ID_PATH;
typedef char VData_Type;

//Judge whether PUSH is succesfully or not! 
#define FAIL_PUSH 0
#define SUCS_PUSH 1

//specify all the ID number of core bound to responding thread
#define DATA_GEN_CORE      0
#define DATA_HANDLER_CORE  1
#define TRANSMIT_CORE      2


//indict the location of each Elem_Data(data block) in a video stream
#define FRONT_FOR_FIRST_NALU 1 
#define MID_FOR_FIRST_NALU   2
#define END_FOR_FIRST_NALU   3

#define FRONT_FOR_MID_NALU   4
#define MID_FOR_MID_NALU     5
#define END_FOR_MID_NALU     6

#define FRONT_FOR_END_NALU   7
#define MID_FOR_END_NALU     8
#define END_FOR_END_NALU     9


//the bytes of control message bytes  in one packet 
#define LEN_CONTRL_MSG 10


using namespace std;


struct Param_Reader {
	char *path_input;
//	int len_block; 
};

//for a block 
struct Elem_Data{
//an order of S*K bytes data
	VData_Type *data;
	int size;
	int id_path;
	int id_nalu;
//specify sequence of nalus in a video segment,
//the first, mid or the last nalu?
	int type_location;
	int type_nalu;
	int S_FEC;
	int K_FEC;
//	int id_segment; 
};

class Data_Manager {
public:	
	vector< queue<struct Elem_Data *> > data_video;

	vector<std::string> data_vec;

	Data_Manager(int max_size);
	Data_Manager();
	~Data_Manager();

	bool data_save(struct Elem_Data *data, ID_PATH id_path);
	struct Elem_Data *data_fetch(ID_PATH id_path);

//friend classes
	friend class Video_Reader;
	friend class Encoder;
	friend class Transmitter;
	friend void partition_nalu(int id_region, VData_Type *cstr, Data_Manager &data_manager);	

//	std::mutex mtx[2];
	
	int buf_size[NUM_PATH];
	int MAX_SIZE[NUM_PATH];

	bool Is_empty(ID_BUF id_buf);
	bool Is_overflow(ID_BUF id_buf);
	bool Push(struct Elem_Data *data_src, ID_PATH id_path);
	struct Elem_Data *Pop(ID_PATH id_path);

private:
//video reader
//	void gen_param_reader(char input_video_path);

//transmitter
//	void Init_param_transmitter(struct Param_Transmitter &param_transmit);
};

#endif







/*

#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <queue>

#include <thread>
#include <mutex>
#include "system_params.h"

typedef int      ID_BUF;
typedef int     ID_PATH;
typedef char struct Elem_Data;

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

struct Elem_Data{
//an order of S*K bytes data
	struct Elem_Data *data;
	int id_nalu;
//specify sequence of nalus in a video segment,
//the first, mid or the last nalu?
	int type_location;
	int S_FEC;
	int K_FEC;
//	int id_segment; 
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

	bool data_save(struct Elem_Data *data, ID_PATH id_path);
	struct Elem_Data *data_fetch(ID_PATH id_path);

	bool Is_empty(ID_BUF id_buf);
	bool Is_overflow(ID_BUF id_buf);
	bool Push(struct Elem_Data *data_src, ID_PATH id_path);
	struct Elem_Data *Pop(ID_PATH id_path);

//video reader
//	void gen_param_reader(char input_video_path);

//transmitter
	void Init_param_transmitter(struct Param_Transmitter &param_transmit);
};

#endif

*/