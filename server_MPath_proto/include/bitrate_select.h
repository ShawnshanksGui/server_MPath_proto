#include "../include/video_reader.h"

class Bitrate_Select {
public:
	int S_FEC;
	int K_FEC;
	
	Bitrate_Select() {}
	~Bitrate_Select() {}

	void bitrate_select_thread_fun(Video_Reader &video_reader); 

private:

};