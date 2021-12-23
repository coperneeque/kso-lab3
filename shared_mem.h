#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#include "fifo_big.h"
#include "fifo_med.h"
#include "lifo_small.h"

int getBigBlock(char* path, int proj_id);
Fifo_big_t* attachBigBlock(int block_id);

#endif
