#ifndef _CODESTREAMING_PASER_H_
#define _CODESTREAMING_PASER_H_

#include "string"
#include "iostream"
#include "fstream"

std::string slurp(std::ifstream &File);

//.h265 code streaming parser
int hevc_parser(string &p, int i);

#endif