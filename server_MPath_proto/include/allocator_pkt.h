class coder_video {
public:

private:
	
};


class VideoReader {
public:	
	queue<char *> data_video[NUM_PATH];

	VideoReader(int max_size);
	VideoReader();
	~VideoReader();

//	void path_decision(int &id_path, double plr[NUM_PATH], 
//		               double RTT[NUM_PATH]);
//	void para_FEC_decision(struct Para_encd &para_encd, double plr);
	
	void video_reader_thread();
	void data_handler_thread();
	void transmit_thread(struct Param_Transmitter param_transmit);

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

//video reader
//	void gen_param_reader(char input_video_path);

//transmitter
	void Init_param_transmitter(struct Param_Transmitter &param_transmit);
};