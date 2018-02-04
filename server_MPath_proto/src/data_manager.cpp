#include "../include/data_manager.h"

using namespace std;
//typedef elem_type char;

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180204
//Description: the thread function which implement data generating procedure  
//Parameter:  CODELINE is equal to encoding symbol size
//		       
//==========================================================================
void Data_Manager::data_gen_thread() {
	FILE *fp;

	Fopen_for_read(&fp,"input_.mp4");
	while(1) {
		data_tmp = new string();
		if(END_FILE == Fread(data_tmp, fp)) break;
 
		data_gen_work(data_tmp);
	}
}

//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: implement main data generating procedure which add mutex signal  
//Parameter:  		       
//==========================================================================
void Data_Manager::data_gen_work(string &data) {
	std::unique_lock<std::mutex> lock(mtx);
//	data = new sring(data_src);
	Push(data);
}
//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180204
//Description: implement main data fetch procedure  
//Parameter:  		       
//==========================================================================
void Data_Manager::data_fetch(string &dst) {
	std::unique_lock<std::mutex> lock(mtx);	
	dst = Pop();
}
//==========================================================================


void Data_Maanager::Push(string &data) {
	if(!Is_overflow()) {
		push();
	}
}

string Data_Manager::Pop() {
	if(!Is_empty()) {
		return pop();
	}
}

void Data_Manager::Is_overflow() {
	return MAX_SIZE == buf_size;
}

void Data_Manager::Is_empty() {
	return 0 == buf_size;
}


Data_Manager::Data_Manager():buf_size(0), MAX_SIZE(100) {

}

Data_Manager::~Data_Manager() {
	while(buf_size--) {
		delete data_video.Pop();
	}
}
