#include "unistd.h"

#include "../include/timer.h"
#include "../include/system_params.h"


#define START 1
#define STOP  0

#define YES   1

extern int Terminal_AllThds;
extern int Terminal_SendThds;
//int Terminal_EncdThds = 0;
extern int Terminal_ReaderThds;

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
void Timer::setTimer_td_func() {
	usleep((long int)(num_timeSlice*len_timeSlice*1000000.0));
	Terminal_AllThds = YES;
}
//==========================================================================