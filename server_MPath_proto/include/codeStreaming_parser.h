#ifndef _CODESTREAMING_PASER_H_
#define _CODESTREAMING_PASER_H_

#include "string"
#include "iostream"
#include "fstream"

#include "video_reader.h"

std::string slurp(std::ifstream &File);

//.h265 code streaming parser
int hevc_parser(string &p, int i, Video_Reader *p_reader);

#endif