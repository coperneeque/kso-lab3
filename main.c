/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "fifo_big.h"
#include "fifo_med.h"
#include "lifo_small.h"
#include "shared_mem.h"
 
#include "simple_test.h"
#include "test_flags.h"

#define SHMEM_FILE ("mem.txt")

int main(int argc, char **argv)
{

#ifdef TEST_FIFO_BIG
    test_FifoBig();
#endif
#ifdef TEST_FIFO_MED
    test_FifoMed();
#endif
#ifdef TEST_LIFO_SMALL
    test_LifoSmall();
#endif

    Fifo_big_t bigFifo;
    initFifoBig(&bigFifo);
    for (size_t i = 0; i < 10; i++)
    {
        putFifoBig(&bigFifo, 51 + i);
    }
    printf("bigFifo before memcpy(): capacity: %u, size: %u\n", bigFifo.capacity, bigFifo.size);

    int blockId = getBigBlock(SHMEM_FILE, 0);
    Fifo_big_t *bigBuffer = attachBigBlock(blockId);

    memcpy(bigBuffer, &bigFifo, sizeof(Fifo_big_t));

    pid_t parentpid = getpid();

    if (fork() == 0)  // child removes 4 from shared memory
    {
        popFifoBig(bigBuffer);
        popFifoBig(bigBuffer);
        popFifoBig(bigBuffer);
        popFifoBig(bigBuffer);
    }
    else  // parent just waits a moment
    {
        sleep(1);
        printf("bigBuffer after memcpy(): capacity: %u, size: %u\n", bigBuffer->capacity, bigBuffer->size);
    }
    
    // shared memory removed by parent only
    if (getpid() == parentpid)
    {
        shmctl(blockId, IPC_RMID, NULL);
    }
    
    return 0;
}
