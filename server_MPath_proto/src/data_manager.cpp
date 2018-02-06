#include "../include/thread_core_affinity_set.h"

#include "../include/data_manager.h"

#include <chrono>

#include "../include/server.h"
#include "../include/common.h"
#include "../include/utility.h"
using namespace std;

static std::mutex mtx;

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180204
//Description: the thread function which simulates data generating procedure  
//Parameter:  CODELINE is equal to encoding symbol size 
//==========================================================================
void Data_Manager::data_gen_thread() {
	FILE *fp;
	Fopen_for_read(&fp, "input.mp4");

	auto startTime = std::chrono::high_resolution_clock::now();

//set thread core affinity and bind core 1 to the current thread
	affinity_set(0);
//
	while(1) {
		//real memory allocation function for data generated 
		data_type *elem_mem_alloc = MALLOC(char, CODELINE);
		if(END_FILE == Fread(elem_mem_alloc, CODELINE, fp)) break;  
		//until push successfully
		while(SUCS_PUSH != data_gen(elem_mem_alloc));

		//records the eclpsing time for TEST_SECONDS
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds> 
                        (endTime - startTime ).count();

		if(duration > TEST_SECONDS*1000000) break;
	}	
}
//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: the thread function which simulates data fetch procedure  
//Parameter:  CODELINE is equal to encoding symbol size
//==========================================================================
void Data_Manager::data_fetch_thread() {
	auto startTime = std::chrono::high_resolution_clock::now();

//set thread core affinity and bind core 1 to the current thread
	affinity_set(1);  
//
	while(1) {
		data_type *data_str = fetch();
		SAFE_FREE(data_str);

		//records the eclpsing time for calculating testing time
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>
        				(endTime - startTime ).count();
        if(duration > TEST_SECONDS*1000000) break;
	}
}
//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: implement main data generating procedure which adds mutex signal  
//Parameter:  		       
//==========================================================================
bool Data_Manager::data_gen(data_type *data) {
	std::unique_lock<std::mutex> lock(mtx);

	return Push(data);
}
//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: implement main data fetch procedure  
//Parameter:  		       
//==========================================================================
data_type *Data_Manager::fetch() {   
	std::unique_lock<std::mutex> lock(mtx);	
	return Pop();
}
//==========================================================================


//safety push into queue
bool Data_Manager::Push(data_type *data_src) {
	if(!Is_overflow()) {
		data_video.push(data_src);
		buf_size += 1;
//debug		
		cout << "Push one time:" << buf_size << endl;
//		printf("%s\n\n", data_video.back());
		return SUCS_PUSH;
	}
	//data buffer overflows, busy waiting for push at next time 
	else 
		return FAIL_PUSH;
}


//safety pop from queue;
data_type *Data_Manager::Pop() {
	if(!Is_empty()) {
		data_type *data_dst = data_video.front();
		data_video.pop();
		buf_size -= 1;
//debug
		cout << "pop one time " << buf_size << endl;
//		printf("%s\n", data_dst);
		return data_dst;
	}
	return nullptr;
}

bool Data_Manager::	Is_overflow() {
	return MAX_SIZE == buf_size;
}

bool Data_Manager::Is_empty() {
	return 0 == buf_size;
}

Data_Manager::Data_Manager(int max_size) {
	buf_size = 0;
	MAX_SIZE = max_size;
}

Data_Manager::~Data_Manager() {
	while(buf_size--) {
		SAFE_FREE(data_video.front()); 
		data_video.pop();
	}
}
Data_Manager::Data_Manager() {
	buf_size = 0;
	MAX_SIZE = 100;
}
//test
int main() {
	Data_Manager dm = Data_Manager(100);

	std::thread worker_data_gen(&Data_Manager::data_gen_thread, &dm);
	std::thread worker_fetch(&Data_Manager::data_fetch_thread,  &dm); 
	worker_data_gen.join();
	worker_fetch.join();

	return 0;
}