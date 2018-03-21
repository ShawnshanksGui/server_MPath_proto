#ifndef _COMMON_H_
#define _COMMON_H_

#include        "ctime"
#include       "cstdio"
#include       "cerrno"
#include      "cstdlib"
#include      "cstring"
#include      "cassert"
#include     "unistd.h"
#include   "sys/time.h"

#include "netinet/in.h"
#include  "arpa/inet.h"
   
#include        <list>
#include    <iostream>
#include     <utility>

//control procedure sending or stopping
#define  STOP                  0
#define READY                  1

//the amount of command parameters required 
#define NUM_PARA               6
 
//finish reading for a video file  
#define END_FILE               0

#define MAX_BUF               10 

//#define TEST_SECONDS         0.1

//#define TOTAL_WORK_THREADS    10


#define MALLOC(type, size) ((type *)malloc(sizeof(type) * (size)))
#define SAFE_FREE(a) {if (a != nullptr) free(a);}

struct Channel_Inf {
//packet loss rate estimated in realtime
	double plr;
//round trip time estimated in realtime
	double RTT;
//available bandwidth estimated in realtime from the receiver's feedback 
	double avail_bw;
};

struct Tile_Num {
	int total_tileNum;
	int fov_tileNum;
	int cushion_tileNum;
	int outmost_tileNum;
};

/*
#define CODELINE             100
#define SYMB_SIZE            100
//original data block size
#define ORIGIN_BLOCK_SIZE    240  
#define ENCD_BLOCK_SIZE      256    
*/

#endif
