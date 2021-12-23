#include "shared_mem.h"

// #include <stdlib.h>
#include <sys/shm.h>

#include "fifo_big.h"
#include "test_flags.h"

int getBigBlock(char* path, int proj_id)
{
    key_t blockKey = ftok(path, proj_id);

    if (blockKey == -1)
    {
            #ifdef MP_DEBUG
        perror("attachBigBlock(): ftok() failed on mem.txt");s
            #endif
        return -1;
    }
    
    int blockId = shmget(blockKey, sizeof(Fifo_big_t), 0644 | IPC_CREAT);
    if (blockId == -1)
    {
        #ifdef MP_DEBUG
        perror("attachBigBlock(): shmget() failed");
        #endif
        return -1;
    }

    return blockId;
}

Fifo_big_t* attachBigBlock(int block_id)
{
    Fifo_big_t* ret = (void*)0;

    ret = shmat(block_id, (void*)0, 0);
        #ifdef MP_DEBUG
    if (ret == (void*)0)
    {
        perror("attachBigBlock(): shmat() failed");
    }
        #endif

    return ret;
}
