
#include "chrono"
#include "iostream"
#include "fstream"
#include "sstream"
#include "string"
#include "ios"

#include "cstdio"

//#include "../include/video_reader.h"

using namespace std;

//for test and debug
//#define ENABLE_DEBUG_PARSER

//for test and debug
#ifdef ENABLE_DEBUG_PARSER

#define REGION_NUM 3
#define FRAME_GOP 50
#define GOP_NUM 2

//the intra pridicted frame
#define I_FRAME 1
//the forward predicted frame
#define P_FRAME 0

struct Nalu_Elem{
    bool frameType;
//the start address for each NALU. 
    int _addr;
//the size for each NALU.
    int _size;
};

struct Nalu_Elem nalu[REGION_NUM][(FRAME_GOP+10)*GOP_NUM];
#endif


#define    HEVC_NAL_TRAIL_N     0
#define    HEVC_NAL_TRAIL_R     1
#define    HEVC_NAL_TSA_N       2
#define    HEVC_NAL_TSA_R       3
#define    HEVC_NAL_STSA_N      4
#define    HEVC_NAL_STSA_R      5
#define    HEVC_NAL_RADL_N      6
#define    HEVC_NAL_RADL_R      7
#define    HEVC_NAL_RASL_N      8
#define    HEVC_NAL_RASL_R      9

#define    HEVC_NAL_BLA_W_LP    16
#define    HEVC_NAL_BLA_W_RADL  17
#define    HEVC_NAL_BLA_N_LP    18
#define    HEVC_NAL_IDR_W_RADL  19
#define    HEVC_NAL_IDR_N_LP    20
#define    HEVC_NAL_CRA_NUT     21

#define    HEVC_NAL_VPS         32
#define    HEVC_NAL_SPS         33
#define    HEVC_NAL_PPS         34

#define    HEVC_NAL_AUD         35
#define    HEVC_NAL_EOS_NUT     36
#define    HEVC_NAL_EOB_NUT     37
#define    HEVC_NAL_FD_NUT      38
#define    HEVC_NAL_SEI_PREFIX  39
#define    HEVC_NAL_SEI_SUFFIX  40


std::string slurp(std::ifstream &File) {
    std::stringstream sstr;
    sstr << File.rdbuf();

    return sstr.str();
}


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei          Date:     20180311.pm
//Description: paser the code stream for a segment, generate some arrays 
//             recording starting address, size for each nalu in this segment    
//Parameter:   id_region specify the ID of region(FOV, cushion and outmost)
//             string obj load the specified pattern we deserve  
//==========================================================================
int hevc_parser(string &p, int id_region) {
    int i = 0;
    int num = 0;  
    int loc = 0;

    int vps  = 0;
    int sps  = 0;
    int pps  = 0;

//the number of I frame(B,P).
    int cnt_irap = 0;
//the number of non-I frame(B,P).
    int cnt_BorP = 0;

    string obj;
//    string tmp;

    char tmp_a[3];
    tmp_a[0] = 0;
    tmp_a[1] = 0;
    tmp_a[2] = 1;
    char *p_str = tmp_a;
    obj.append(p_str, 3);
//    obj.append(tmp_b);

    auto startTime = std::chrono::high_resolution_clock::now();
//    for (i = 0; i < p.length(); i++) {
    while(i < p.length()) {
        loc = p.find(obj, i);
        if(loc == string::npos) {
            printf("parser procedure is finished\n");
            break;
        }
        i = loc+3;
        int type = (p[loc+3] & 0x7E) >> 1;
        switch (type) {
        case HEVC_NAL_VPS:       
        {
            vps++;
            nalu[id_region][num].frameType = I_FRAME;
//            nalu[id_region][num]._addr = i-4;
            nalu[id_region][num]._addr=(p[loc-1]==0) ? (i-4) : (i-3);
            if(num > 0) {
                //derive the size of the last one nalu;
                nalu[id_region][num-1]._size = nalu[id_region][num]._addr-nalu[id_region][num-1]._addr;
            }
            num++;
            break;
        }
        case HEVC_NAL_SPS:      
        {
        /*
            sps++;
            nalu[id_region][num]._size = i - nalu[id_region][num]._addr;
            num++; 
            nalu[id_region][num]._addr = i;;  
        */ 
            break;
        }
        case HEVC_NAL_PPS:        
        { 
        /*
            pps++;
            nalu[id_region][num]._size = i - nalu[id_region][num]._addr;  
            num++;
            nalu[id_region][num]._addr = i;;                
         */
            break;
        } 
        case HEVC_NAL_TRAIL_N:
        case HEVC_NAL_TRAIL_R:
        case HEVC_NAL_TSA_N:
        case HEVC_NAL_TSA_R:
        case HEVC_NAL_STSA_N:
        case HEVC_NAL_STSA_R:
        case HEVC_NAL_RADL_N:
        case HEVC_NAL_RADL_R:
        case HEVC_NAL_RASL_N:
        case HEVC_NAL_RASL_R:
        {   
            cnt_BorP++;
            nalu[id_region][num]._addr = (p[loc-1]==0) ? (i-4) : (i-3);
            nalu[id_region][num].frameType  = P_FRAME;
            nalu[id_region][num-1]._size = nalu[id_region][num]._addr - nalu[id_region][num-1]._addr;

//            nalu[id_region][num]._addr = i-3;
            num++;             
            break;
        }
//
        case HEVC_NAL_BLA_N_LP:
        case HEVC_NAL_BLA_W_LP:
        case HEVC_NAL_BLA_W_RADL:
        case HEVC_NAL_CRA_NUT:
        case HEVC_NAL_IDR_N_LP:
        case HEVC_NAL_IDR_W_RADL: 
        {
            cnt_irap++;
        /* 
            nalu[id_region][num].frameType  = I_FRAME;
            nalu[id_region][num]._size = i - nalu[id_region][num]._addr;
            num++;
            nalu[id_region][num]._addr = i-3;
        */
//            std::cout << "addr of the " << cnt_irap <<
//                       "'th I frame is " << i;
//            std::cout << std::endl; 
            break;
        }
        }
    }
//count the isze of the video segment's last NALU 
    nalu[id_region][num-1]._size = p.length() - 1 - nalu[id_region][num-1]._addr;
//

//records the eclpsing time for calculating testing time
    auto endTime  = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                    (endTime - startTime ).count();

    printf("the time cost is %ld us\n", duration);

#ifdef ENABLE_DEBUG_PARSER
{
    printf("the number of vps,sps,pps,I_frame,P_frame is %d,%d,%d,%d,%d,respectively\n\n",
           vps, sps, pps, cnt_irap, cnt_BorP);
    
    printf("the address of all nalus is following:\n");
    for(int i = 0; i < (FRAME_GOP+10)*GOP_NUM; i++) {
        printf("%d  ", nalu[id_region][i]._addr);
    }
    printf("\n\n");

    printf("the nalu size is following:\n");
    for(int i = 0; i < (FRAME_GOP+10)*GOP_NUM; i++) {
        printf("%d  ", nalu[id_region][i]._size);
    }
    printf("\n");
}
#endif
}
//==========================================================================


#ifdef ENABLE_DEBUG_PARSER
int main() {
    int flag_video = 0;
//    std::string _input;
    std::ifstream File;
    std::string inString;

    File.open("../../../video_test/machu_picchu_a_s111_non_B.265", std::ios::in);
//    File.open("../../../machu_picchu_8k_a_s111.265", std::ios::in);
//    File.open("input_non_b.265", std::ios::in);

    inString = slurp(File);

 //   inString.find()
    flag_video = hevc_parser(inString, 1);

//    printf("\nflag_video is %d\n", flag_video);
//  std::cout << inString;

    return 0;    
}
#endif
