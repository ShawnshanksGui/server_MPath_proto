#include "../include/data_manager.h"

#include <queue>
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
//		       
//==========================================================================
void Data_Manager::data_gen_thread() {
	FILE *fp;
	Fopen_for_read(&fp, "input.mp4");

	auto startTime = std::chrono::high_resolution_clock::now();

	while(1) {
		//real memory allocation function for data generated 
		data_type *data_tmp = MALLOC(char, CODELINE);
		if(END_FILE == Fread(data_tmp, CODELINE, fp)) break;  

		while(SUCS_PUSH == data_gen(data_tmp));

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
	data_type *data_str = nullptr;
	auto startTime = std::chrono::high_resolution_clock::now();

	while(1) {
//		data_str = nullptr;
		//until fetch successfully
		data_str = fetch();

		//records the eclpsing time for TEST_SECONDS
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>
        				(endTime - startTime ).count();
        if(duration > TEST_SECONDS*1000000) break;
//debug
//		cout << "pop one time " << buf_size << endl;
		//data popped out from the front of queue
//		cout << data_str << endl;
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
//debug
//	cout << "Push one time:" << buf_size << endl;
	//data generated enter at th back. 
//	cout << data_video.back() << endl; 

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
		return data_dst;
	}
	return nullptr;
}

bool Data_Manager::Is_overflow() {
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
/*
	pthread_t threads[2];

	pthread_create(&threads[0], NULL, dm.data_gen_thread, NULL);
	pthread_create(&threads[1], NULL, dm.data_fetch_thread, NULL);	

*/
	std::thread worker_data_gen(&Data_Manager::data_gen_thread, &dm);
	std::thread worker_fetch(&Data_Manager::data_fetch_thread,  &dm); 
	worker_data_gen.join();
	worker_fetch.join();

	return 0;
}