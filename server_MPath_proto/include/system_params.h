#ifndef _SYSTEM_PARAMS_H_
#define _SYSTEM_PARAMS_H_

//Specify total number of paths, including a wifi and a cellular channel,
//also equal to the number of sending buffers
#define NUM_PATH 2 

//the total number of tile for a frame
#define TILE_NUM_TOTAL  72
//the number of tiles for a frame's different region
#define FOV_TILE_NUM     4*3
#define CUSHION_TILE_NUM 6*5-4*3
#define OUTMOST_TILE_NUM 8*9-6*5

//specify the bitrate of video segment
//#define 

//define the number of GOP in a video segment(how much seconds) 
#define GOP_NUM 1 

//define the number of frames in one GOP
#define FRAME_GOP 30

//define the number of regions(FOV, non-FOV region), 
//which test video is conposed of 
#define REGION_NUM 3


//FEC default parameter
#define SYMBOL_FEC 1000

//the bytes of control message bytes  in one packet 
#define LEN_CONTRL_MSG 18

//specify the vailable number of video tile's quality    
#define BITRATE_TYPE_NUM 3

//it work when encoding really. 
#define RATE 5

//for seting FEC_params
#define STEP_VALUE 0.005

//running time for program 
#define NUM_TIMESLICE 1
#define LEN_TIMESLICE 1


//specidies whether FFT_RS or RS
#define ENABLE_FFT_RS


#endif