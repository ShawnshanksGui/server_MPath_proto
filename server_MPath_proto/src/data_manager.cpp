#include "../include/thread_core_affinity_set.h"

#include "../include/data_manager.h"

#include <chrono>

#include "../include/server.h"
#include "../include/common.h"
#include "../include/utility.h"

extern "C"
{
	#include "../include/rs_fft.h"
}

using namespace std;

static std::mutex mtx;

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180204
//Description: the thread function which simulates data generating procedure  
//Parameter:  SYMB_SIZE is equal to encoding symbol size 
//==========================================================================
void Data_Manager::data_gen_thread() {
	int id_path  = 0;
	int _count   = 0;
	int len_read = 0;
//original data block size.
	int block_size = 200;

	FILE *fp;
	Fopen_for_read(&fp, "input.mp4");

	auto startTime = std::chrono::high_resolution_clock::now();

//set thread core affinity and bind core 1 to the current thread
	affinity_set(0);
//
	while(1) {
		//real memory allocation function for data generated 
		data_type *elem_mem_alloc = MALLOC(char, SYMB_SIZE*block_size);
		len_read = Fread(elem_mem_alloc, SYMB_SIZE*block_size, fp);
		if(END_FILE == len_read) break;  

		//until pushed and saved successfully
		while(SUCS_PUSH != data_save(elem_mem_alloc, id_path));

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
//Parameter:  SYMB_SIZE is equal to encoding symbol size
//==========================================================================
void Data_Manager::data_handler_thread() {
	int id_path = 0;

	data_type *data_take  = nullptr;	
	data_type *encd_block = nullptr;

	//encd_Qfer save data from origin data queue
	
	auto startTime = std::chrono::high_resolution_clock::now();

	//set thread core affinity and bind core 1 to the current thread
	affinity_set(1);  
//
	while(1) {
		char *tmp_data = MALLOC(char, SYMB_SIZE*ENCD_BLOCK_SIZE);
		while(nullptr == (data_take = data_fetch(id_path)));

//		CopyFromDataQ2EncdQ(encd_Q[id_path], tmp_data, SYMB_SIZE*ENCD_BLOCK_SIZE);
//=======================================================
		encd_block = encode_FFT_RS(data_take, para_encd);
		SAFE_FREE(data_take);
//=======================================================
		encd_Q[id_path].push(encd_block);

//=======================================================
//		data_handle_procedure

//=======================================================
//		SAFE_FREE(data_str);

		//records the eclpsing time for calculating testing time
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>
        				(endTime - startTime ).count();
        if(duration > TEST_SECONDS*1000000) break;
	}
}

void Data_Manager::sender_thread_a(){
	
	while(1) {
		char *data = encd_Q[id_path].front(); 
		encd_Q[id_path].pop();	
		for(int i = 0; i < ENCD_BLOCK_SIZE; i++) {
			Udp_send(encd_block[i*SYMBOL_SIZE], SYMBOL_SIZE);
		}
	}	
}

void Data_Manager::sender_thread_a(){
	
	while(1) {
		char *data = encd_Q[id_path].front(); 
		encd_Q[id_path].pop();	
		for(int i = 0; i < ENCD_BLOCK_SIZE; i++) {
			Udp_send(encd_block[i*SYMBOL_SIZE], SYMBOL_SIZE);
		}
	}	
}
//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: implement main data generating procedure which adds mutex signal  
//Parameter:  		       
//==========================================================================
bool Data_Manager::data_save(data_type *data, ID_PATH id_path) {
	std::unique_lock<std::mutex> lock(mtx);

	return Push(data, id_path);
}
//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: implement main data fetch procedure  
//Parameter:  		       
//==========================================================================
data_type *Data_Manager::data_fetch(ID_PATH id_path) {   
	std::unique_lock<std::mutex> lock(mtx);	

	return Pop(id_path);
}
//==========================================================================


//safety push into one queue(default the first queue)
bool Data_Manager::Push(data_type *data_src, ID_PATH id_path) {
	if(!Is_overflow(id_path)) {
		data_video[id_path].push(data_src);
		buf_size[id_path] += 1;
//debug		
		cout << "Push one time:" << buf_size[id_path] << endl;
//		printf("%s\n\n", data_video.back());
		return SUCS_PUSH;
	}
	//data buffer overflows, busy waiting for push at next time 
	else 
		return FAIL_PUSH;
}


//safety pop from queue;
data_type *Data_Manager::Pop(ID_PATH id_path) {
	if(!Is_empty(id_path)) {
		data_type *data_dst = data_video[id_path].front();
		data_video[id_path].pop();
		buf_size[id_path] -= 1;
//debug
		cout << "pop one time " << buf_size[id_path] << endl;
//		printf("%s\n", data_dst);
		return data_dst;
	}
	return nullptr;
}

bool Data_Manager::Is_overflow(ID_BUF id_buf) {
//	return MAX_SIZE == (buf_size[id_buf]);
	return buf_size[id_buf] == MAX_SIZE[id_buf];
}

bool Data_Manager::Is_empty(ID_BUF id_buf) {

	return 0 == (buf_size[id_buf]);
}

Data_Manager::Data_Manager(int max_size) {
	for(int i = 0; i < NUM_PATH; i++) {
		buf_size[i] = 0;
		MAX_SIZE[i] = max_size;
	}
}

Data_Manager::Data_Manager() {
	for(int i = 0; i < NUM_PATH; i++) {
		buf_size[i] = 0;
		MAX_SIZE[i] = 100;
	}
}

Data_Manager::~Data_Manager() {
	for(int i = 0; i < NUM_PATH; i++) {
		while(buf_size[i]--) {
			SAFE_FREE(data_video[i].front()); 
			data_video[i].pop();
		}
	}
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