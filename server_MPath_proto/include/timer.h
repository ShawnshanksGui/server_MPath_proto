
class Timer {
public:
	int num_timeSlice;
	int len_timeSlice;

	Timer();

	~Timer() {}

	void setTimer_td_func(int &terminalFlag, 
					 	  int &startFlag_one_timeSlice);
private:

};