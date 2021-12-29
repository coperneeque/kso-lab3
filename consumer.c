/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021-z                                                           *
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
    int run = 5;
    int need;
    int pid = getpid();

// attach to existing (hopefully]) big buffer
    int bigBlockId = getMemBlock(SHMEM_FILE, 0, sizeof(Fifo_big_t));
    Fifo_big_t *bigBuffer = attachMemBlock(bigBlockId);
    printf("Consumer:\t%u\tAttached to shared big buffer\n", pid);
    printf("Consumer:\t%u\t", pid);
    printFifoBig(bigBuffer);

    srandom(time(NULL));

    while(run)
    {
        // run = random() % 30;
        --run;
        need = random() % 10;  // how much data needed by consumer
        printf("Consumer:\t%u\trun: %u, need: %u. \n", pid, run, need);
        // while (need <= bigBuffer->size)
        while (need > 0)
        {
            printf("Consumer:\t%u\trun: %u, need: %u. ", pid, run, need);
            sem_wait(&bigBuffer->mutex);  // access the buffer
            if (need <= bigBuffer->size)  // also case when size==0
            {
                if (need > bigBuffer->chunk)
                {
                    printf("Consuming %u units\n", bigBuffer->chunk);
                    for (size_t i = 0; i < bigBuffer->chunk; i++)
                    {
                        sem_wait(&bigBuffer->semFull);
                        popFifoBig(bigBuffer);
                        sem_post(&bigBuffer->semEmpty);
                    }
                    need -= bigBuffer->chunk;
                }
                else
                {
                    printf("Consuming %u units\n", need);
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

    printf("Consumer:\t%u\tFinishing:\n", pid);
    printf("Consumer:\t%u\t", pid);
    printFifoBig(bigBuffer);
    return 0;
}
