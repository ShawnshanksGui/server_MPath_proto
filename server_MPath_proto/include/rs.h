#ifndef MYRS_H
#define MYRS_H

#include "encoder.h"
#include "decoder.h"

void *rs_encode(struct Encoder *encoder, void **args);

void *rs_decode(struct Decoder *decoder, void **args);

#endif