#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#include <stddef.h>

#include "fifo_big.h"
#include "fifo_med.h"
#include "lifo_small.h"

int getMemBlock(char* path, int proj_id, size_t size);
void* attachMemBlock(int block_id);
// int getMedBlock(char* path, int proj_id);
// Fifo_med_t* attachMedBlock(int block_id);

#endif
