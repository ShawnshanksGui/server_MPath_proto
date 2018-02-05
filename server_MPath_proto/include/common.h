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

#include <iostream>

typedef struct sockaddr SA;

#define MALLOC(type, size) ((type *)malloc(sizeof(type) * (size)))
#define SAFE_FREE(a) if (a != NULL) free(a);