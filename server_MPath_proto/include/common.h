#include       "cstdio"
#include      "cstring"
#include       "cerrno"
#include     "unistd.h"
#include      "cstdlib"
#include        "ctime"
#include      "cassert"
#include   "sys/time.h"

#include "netinet/in.h"
#include  "arpa/inet.h"

typedef struct sockaddr SA;

#define MALLOC(type, size) (malloc(sizeof(type) * (size)))
#define SAFE_FREE(a) if (a != NULL) free(a);