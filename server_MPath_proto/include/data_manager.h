#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_


#include <queue>
#include <list>

#include <thread>
#include <mutex>

#include "common.h"

typedef char data_type;

using namespace std;

class Data_Manager {
public:	
	queue<char *> data_video;

	Data_Manager(int max_size);
	Data_Manager();
	~Data_Manager();

	void data_gen_thread();
	void data_fetch_thread();

private:
	void data_gen(data_type *data);
	data_type *fetch();

	bool Is_empty();
	bool Is_overflow();
	void Push(data_type *data_src);
	data_type *Pop();
	int buf_size;
	int MAX_SIZE;

};


#endif