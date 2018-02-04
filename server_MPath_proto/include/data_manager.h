#include <string>
#include <thread>
#include <mutex>

#include "common.h"

class Data_Manager {
public:	
	std::queque<std::string> data_video;

	void data_gen_thread();
	void data_fetch(string &dst);
	void data_gen_work(string &data);
private:
	Is_empty();
	Is_overflow();
	Data_Manager();
	~Data_Manager();
	int buf_size;
	int MAX_SIZE;

};
