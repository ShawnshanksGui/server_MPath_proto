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

#define SUCS_PUSH 1
#define FAIL_PUSH 0

#define MALLOC(type, size) ((type *)malloc(sizeof(type) * (size)))
#define SAFE_FREE(a) {if (a != nullptr) free(a);}