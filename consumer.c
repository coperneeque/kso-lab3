/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "shared_mem.h"
#include "fifo_big.h"
#include "fifo_med.h"
#include "lifo_small.h"

int main()
{
    int run = 1;
    int need;

// attach to existing (hopefully]) big buffer
    int bigBlockId = getMemBlock(SHMEM_FILE, 0, sizeof(Fifo_big_t));
    Fifo_big_t *bigBuffer = attachMemBlock(bigBlockId);

    srandom(time(NULL));

    while(run) {
        run = random() % 30;
        need = random() % 10;  // how much data needed by consumer
        // while (need <= bigBuffer->size)
        while (need > 0)
        {
            printf("Consumer pid %u, run == %u, need == %u\n", getpid(), run, need);
            sem_wait(&bigBuffer->mutex);  // access the buffer
            if (need <= bigBuffer->size)  // also case when size==0
            {
                if (need > bigBuffer->chunk)
                {
                    for (size_t i = 0; i < bigBuffer->chunk; i++)
                    {
                        sem_wait(&bigBuffer->semFull);
                        popFifoBig(bigBuffer);
                        sem_post(&bigBuffer->semEmpty);
                    }
                    need -= bigBuffer->chunk;
                    // sem_post(&bigBuffer->mutex);lkjl
                }
                else
                {
                    for (size_t i = 0; i < need; i++)
                    {
                        sem_wait(&bigBuffer->semFull);
                        popFifoBig(bigBuffer);
                        sem_post(&bigBuffer->semEmpty);
                    }
                    need = 0;
                }
            }
            sem_post(&bigBuffer->mutex);
            sleep(1);
        }        
    }

    return 0;
}
