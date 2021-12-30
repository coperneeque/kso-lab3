/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021-z                                                           *
 * lab3                                                                 *
 ************************************************************************/
#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#include <stddef.h>

// #include "fifo_big.h"
// #include "fifo_med.h"
// #include "lifo_small.h"


#define SHMEM_FILE ("mem.txt")

int getMemBlock(char* path, int proj_id, size_t size);
void* attachMemBlock(int block_id);
// int getMedBlock(char* path, int proj_id);
// Fifo_med_t* attachMedBlock(int block_id);

#endif
