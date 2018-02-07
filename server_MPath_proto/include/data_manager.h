#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <queue>

#include <thread>
#include <mutex>

typedef char data_type;
typedef int    ID_PATH;
typedef int     ID_BUF;

//Judge whether PUSH is succesfully or not! 
#define SUCS_PUSH 1
#define FAIL_PUSH 0

//Specify total amount of paths(or buffers)
#define NUM_PATH 2 

using namespace std;

class Data_Manager {
public:	
	queue<char *> data_video[NUM_PATH];

	Data_Manager(int max_size);
	Data_Manager();
	~Data_Manager();

	void data_gen_thread();
	void data_handler_thread();

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
};


#endif