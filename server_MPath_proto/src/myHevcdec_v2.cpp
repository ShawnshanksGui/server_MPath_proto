#include "iostream"
#include "fstream"
#include "sstream"
#include "string"
#include "ios"

#include "cstdio"

//#include "../include/video_reader.h"

using namespace std;

//for test and debug
#define ENABLE_DEBUG

//for test and debug
#ifdef ENABLE_DEBUG

#define REGION_NUM 3
#define FRAME_GOP 50
#define GOP_NUM 2

//the intra pridicted frame
#define I_FRAME 0
//the forward predicted frame
#define P_FRAME 1

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


int hevc_parser(string &p, int id_region) {
    int i = 0;
    int num = 0;
    uint32_t code = -1;    

//the start address for each NALU 
//    int addr_nalu[40] = {0};
//the size for each NALU
//    int size_nalu[40] = {0};

    int vps  = 0;
    int sps  = 0;
    int pps  = 0;

//the number of I frame(B,P).
    int cnt_irap = 0;
//the number of non-I frame(B,P).
    int cnt_BorP = 0; 

    for (i = 0; i < p.length(); i++) {
        code = (code << 8) + p[i];
        if ((code & 0xffffff00) == 0x100) {
            char nal2 = p[i + 1];
            int type = (code & 0x7E) >> 1;

            if (code & 0x81) // forbidden and reserved zero bits
                return 0;

            if (nal2 & 0xf8) // reserved zero
                return 0;

            switch (type) {
            case HEVC_NAL_VPS:       
            {
                vps++;
                nalu[id_region][num].frameType = I_FRAME;
                nalu[id_region][num]._addr = i-4;
                if(num > 0) {
                    //derive the size of the last one nalu;
                    nalu[id_region][num-1]._size=i-4-nalu[id_region][num-1]._addr;
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
                nalu[id_region][num-1]._size = i-3-nalu[id_region][num-1]._addr;
                nalu[id_region][num].frameType  = P_FRAME;
                nalu[id_region][num]._addr = i-3;
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
            /*
                cnt_irap++;
                nalu[id_region][num].frameType  = I_FRAME;
                nalu[id_region][num]._size = i - nalu[id_region][num]._addr;
                num++;
                nalu[id_region][num]._addr = i-3;
            */
//                std::cout << "addr of the " << cnt_irap <<
//                           "'th I frame is " << i;
//                std::cout << std::endl; 
                break;
            }
            }
        }
    }
//count the isze of the video segment's last NALU 
    nalu[id_region][num-1]._size = i - 1 - nalu[id_region][num-1]._addr;
//

#ifdef ENABLE_DEBUG
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
//    if (vps && sps && pps && cnt_irap)
//        return 1;
//    return 0;
}

#ifdef ENABLE_DEBUG
int main() {
    int flag_video = 0;
//    std::string _input;
    std::ifstream File;
    std::string inString;

//    File.open("machu_picchu_8k_a_s111.265", std::ios::in);
    File.open("machu_piccu_8k.265", std::ios::in);
//    File.open("input_non_b.265", std::ios::in);
//    cin >> _input;
//    File.open(_input, std::ios::in);
    inString = slurp(File);

    flag_video = hevc_parser(inString, 1);

//    printf("\nflag_video is %d\n", flag_video);
//  std::cout << inString;

    return 0;    
}
#endif





/*
 * RAW HEVC video demuxer
 * Copyright (c) 2013 Dirk Farin <dirk.farin@gmail.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
/*
#include "libavcodec/hevc.h"

#include "avformat.h"
#include "rawdec.h"


static int hevc_probe(AVProbeData *p)
{
    uint32_t code = -1;
    int vps = 0, sps = 0, pps = 0, cnt_irap = 0;
    int i;

    for (i = 0; i < p->buf_size - 1; i++) {
        code = (code << 8) + p->buf[i];
        if ((code & 0xffffff00) == 0x100) {
            uint8_t nal2 = p->buf[i + 1];
            int type = (code & 0x7E) >> 1;

            if (code & 0x81) // forbidden and reserved zero bits
                return 0;

            if (nal2 & 0xf8) // reserved zero
                return 0;

            switch (type) {
            case HEVC_NAL_VPS:        vps++;  break;
            case HEVC_NAL_SPS:        sps++;  break;
            case HEVC_NAL_PPS:        pps++;  break;
            case HEVC_NAL_BLA_N_LP:
            case HEVC_NAL_BLA_W_LP:
            case HEVC_NAL_BLA_W_RADL:
            case HEVC_NAL_CRA_NUT:
            case HEVC_NAL_IDR_N_LP:
            case HEVC_NAL_IDR_W_RADL: cnt_irap++; break;
            }
        }
    }

    if (vps && sps && pps && cnt_irap)
        return AVPROBE_SCORE_EXTENSION + 1; // 1 more than .mpg
    return 0;
}

FF_DEF_RAWVIDEO_DEMUXER(hevc, "raw HEVC video", hevc_probe, "hevc,h265,265", AV_CODEC_ID_HEVC)
*/