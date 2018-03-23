#include "unistd.h"

#include "../include/timer.h"
#include "../include/system_params.h"


#define START 1
#define STOP  0


Timer::Timer() {
	num_timeSlice = NUM_TIMESLICE;
	len_timeSlice = LEN_TIMESLICE;
}

//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei         Date:        20180305
//Description: the function which controls the tempo of all the processes 
//Parameter:   len_timeSlice denotes the lenth of one time slice
//             (of one video segment);
//             startFlag_one_timeSlice directly control the switch of all 
//             the processes; 
//==========================================================================
void Timer::setTimer_td_func(int &terminalFlag, 
							 int &startFlag_one_timeSlice) {
	while(num_timeSlice--) {
		startFlag_one_timeSlice = START;
		usleep(len_timeSlice*1000000);
		startFlag_one_timeSlice = STOP;
	}
//equal to YES
	terminalFlag = STOP;

}
//==========================================================================