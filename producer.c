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


#define PROD_CAP    10
#define RANGE       100

int main()
{
    int run = 5;
    int produced;
    int products[PROD_CAP];
    int pid = getpid();

// attach to existing (hopefully]) big buffer
    int bigBlockId = getMemBlock(SHMEM_FILE, 0, sizeof(Fifo_big_t));
    Fifo_big_t *bigBuffer = attachMemBlock(bigBlockId);
    printf("Producer:\t%u\tAttached to shared big buffer:\n", pid);
    printf("Producer:\t%u\t", pid);
    printFifoBig(bigBuffer);

    srandom(time(NULL));

    while(run)
    {
        // run = random() % 30;
        --run;
        produced = random() % PROD_CAP;  // how much is produced
        for (size_t i = 0; i < produced; i++)  // simulate production
        {
            products[i] = random() % RANGE;
        }
        
        printf("Producer:\t%u\trun: %u, produced: %u units\n", pid, run, produced);
        while (produced > 0)
        {
            printf("Producer:\t%u\trun: %u, to insert: %u units. ", pid, run, produced);
            sem_wait(&bigBuffer->mutex);  // access the buffer
            if (produced <= bigBuffer->capacity - bigBuffer->size)  // is there space in the buffer
            {
                if (produced > bigBuffer->chunk)
                {
                    printf("Inserting %u units\n", bigBuffer->chunk);
                    for (size_t i = 0; i < bigBuffer->chunk; i++)
                    {
                        sem_wait(&bigBuffer->semEmpty);
                        putFifoBig(bigBuffer,products[i]);
                        sem_post(&bigBuffer->semFull);
                    }
                    produced -= bigBuffer->chunk;
                }
                else
                {
                    printf("Inserting %u units\n", produced);
                    for (size_t i = 0; i < produced; i++)
                    {
                        sem_wait(&bigBuffer->semEmpty);
                        putFifoBig(bigBuffer,products[i]);
                        sem_post(&bigBuffer->semFull);
                    }
                    produced = 0;
                }
            }
            sem_post(&bigBuffer->mutex);
            // sleep(1);
        }        
    }

    printf("Producer:\t%u\tFinishing:\n", pid);
    printf("Producer:\t%u\t", pid);
    printFifoBig(bigBuffer);

    return 0;
}
