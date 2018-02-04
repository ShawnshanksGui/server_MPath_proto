#include       "cstdio"
#include        "ctime"
#include       "cerrno"
#include      "cstdlib"
#include      "cstring"
#include      "cassert"

#include     "unistd.h"
#include   "sys/time.h"

#include "netinet/in.h"
#include  "arpa/inet.h"

typedef struct sockaddr SA;

#define MALLOC(type, size) (malloc(sizeof(type) * (size)))
#define SAFE_FREE(a) if (a != NULL) free(a);