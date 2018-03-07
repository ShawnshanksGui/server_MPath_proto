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

//define the number of frames in one GOP
#define FRAME_GOP 15

//define the number of regions(FOV, non-FOV region), 
//which test video is conposed of 
#define REGION_NUM 3



//specify the vailable number of video tile's quality    
#define BITRATE_TYPE_NUM 3