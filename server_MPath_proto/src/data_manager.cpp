#include "../include/thread_core_affinity_set.h"

#include "../include/data_manager.h"

#include <chrono>
#include <memory>

#include "../include/common.h"
#include "../include/myUtility.h"



using namespace std;

//for debugging
//#define ENABLE_DEBUG_D_MAMAGER

Data_Manager::
Data_Manager(int max_size):data_video(NUM_PATH) {
	for(int i = 0; i < NUM_PATH; i++) {
		buf_size[i] = 0;
		MAX_SIZE[i] = max_size;
	}
}

Data_Manager::
Data_Manager():data_video(NUM_PATH) {
	for(int i = 0; i < NUM_PATH; i++) {
		buf_size[i] = 0;
		MAX_SIZE[i] = 1000;
	}
}


Data_Manager::
~Data_Manager() {
	printf("\nEntering the deconstructor of Data_Manager\n");

	for (int i = 0; i < this->data_vec.size(); i++) {
		delete [] this->data_vec[i];
	}

	printf("finish the deconstructor of Data_Manager\n");
/*
	for(int i = 0; i < NUM_PATH; i++) {
		int num_elem = buf_size[i];
		while(num_elem--) {
			struct Elem_Data *tmp = data_video[i].front();
			printf("%d %d %d %d\n", tmp->size, tmp->id_path, tmp->id_nalu, tmp->type_location);
			SAFE_FREE(data_video[i].front()); 
			data_video[i].pop();
		}
	}
*/
/*
	while(data_video.size() > 0) {
		while(data_video.back().size() > 0) {
			struct Elem_Data *tmp = data_video.back().front();
			printf("%d %d %d %d\n", tmp->size, tmp->id_path, tmp->id_nalu, tmp->type_location);
			SAFE_FREE(tmp); 
			data_video.back().pop();
		}
		data_video.pop_back();
	}
	while(data_vec.size() > 0) {
		SAFE_FREE(data_vec.back());
		data_vec.pop_back();		
	}
*/
}

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: implement main data generating procedure which adds mutex signal  
//Parameter:  		       
//==========================================================================
bool Data_Manager::
data_save(shared_ptr<struct Elem_Data> data, ID_PATH id_path) {
//	std::unique_lock<std::mutex> lock(mtx[id_path]);

	return Push(data, id_path);
}
//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: implement main data fetch procedure  
//Parameter:  		       
//==========================================================================
shared_ptr<struct Elem_Data> Data_Manager::
data_fetch(ID_PATH id_path) {   
//	std::unique_lock<std::mutex> lock(mtx[id_path]);	
	return Pop(id_path);
}
//==========================================================================


//safely push into one queue(default the first queue)
bool Data_Manager::
Push(shared_ptr<struct Elem_Data> data_src, ID_PATH id_path) {
	if(!Is_overflow(id_path)) {
		data_video[id_path].push(data_src);
		buf_size[id_path] += 1;
//debug
		cout << "\nPush one time, the buf_size = " << buf_size[id_path] << endl;
//		printf("%s\n\n", data_video.back());
		return SUCS_PUSH;
	}
	//data buffer overflows, busy waiting for push at next time 
	else 
		return FAIL_PUSH;
}

//safety pop from queue;
shared_ptr<struct Elem_Data> Data_Manager::
Pop(ID_PATH id_path) {
	if(!Is_empty(id_path)) {
		shared_ptr<struct Elem_Data> data_dst = data_video[id_path].front();
		data_video[id_path].pop();
		buf_size[id_path] -= 1;
//debug
		cout << "\npop one time, remain buf_size = " << buf_size[id_path] << endl;
//		printf("%s\n", data_dst);
		return data_dst;
	}
	return nullptr;
}

bool Data_Manager::
Is_overflow(ID_BUF id_buf) {
//	return MAX_SIZE == (buf_size[id_buf]);
	return buf_size[id_buf] == MAX_SIZE[id_buf];
}

bool Data_Manager::Is_empty(ID_BUF id_buf) {

	return 0 == (buf_size[id_buf]);
}





#ifdef  ENABLE_DEBUG_D_MAMAGER
//test
int main() {
	struct Param_Transmitter param_transmit;
	Init_param_transmitter(param_transmit);

	Data_Manager dm = Data_Manager(100);

	std::thread video_read_worker(&Data_Manager::video_reader_thread,  &dm);
	std::thread fetch_worker(&Data_Manager::data_handler_thread, &dm); 
	std::thread transmit_worker(&Data_Manager::transmit_thread, &dm, 
		                        param_transmit);
	worker_data_gen.join();
	data_handler_thread.join();
	transmit_worker.join();

	return 0;
}

#endif
//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: the thread function which simulates data fetch procedure  
//Parameter:   SYMB_SIZE is equal to the encoding symbol size
//==========================================================================
/*
void Data_Manager::data_handler_thread() {
	int id_path = 0;
	struct Elem_Data *data_take  = nullptr;	
	struct Elem_Data *encd_block = nullptr;
//decide encoding parameters
	struct Param_Encd param_encd  = {1000, 240};
//	decision_para_FEC(param_encd);
//
	auto startTime = std::chrono::high_resolution_clock::now();

//set thread core affinity and bind the current thread to core 1
	affinity_set(DATA_HANDLER_CORE);  
//
	while(1) {
//		char *tmp_data = MALLOC(char, SYMB_SIZE*ENCD_BLOCK_SIZE);
		while(nullptr == (data_take = data_fetch(id_path)));

//================================================================
		encd_block = encode_FFT_RS(data_take, param_encd);
		SAFE_FREE(data_take);
//path decision of packets, store encoding data into send_queue
		send_Q[id_path].push(encd_block);
//================================================================

//records the eclpsing time for calculating testing time
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>
        				(endTime - startTime ).count();
        if(duration > TEST_SECONDS*1000000) break;
	}
}
*/
//==========================================================================
