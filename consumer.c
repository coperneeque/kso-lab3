/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021-z                                                           *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "fifo_big.h"
#include "fifo_med.h"
#include "lifo_small.h"
#include "shared_mem.h"
#include "textcolour.h"


#define NEED_CAP    10
#define USEC        1000000

int main(int argc, char **argv)
{
    int run = 5;
    int need;
    int toConsume;
    int pid = getpid();

// attach to existing (hopefully]) big buffer
    int bigBlockId = getMemBlock(SHMEM_FILE, 0, sizeof(Fifo_big_t));
    Fifo_big_t *bigBuffer = attachMemBlock(bigBlockId);
    
    textcolour(0, RED, BLACK); printf("Consumer:\t%u\tAttached to shared big buffer:\n", pid);
    textcolour(0, RED, BLACK); printf("Consumer:\t%u\t", pid);
    textcolour(0, RED, BLACK); printFifoBig(bigBuffer);

    srandom(time(NULL));

    while(run)
    {
        // run = random() % 30;
        --run;
        need = random() % NEED_CAP;  // how much data needed by consumer
        textcolour(0, RED, BLACK); printf("Consumer:\t%u\trun: %u, need: %u. \n", pid, run, need);
        while (need > 0)
        {
            textcolour(0, RED, BLACK); printf("Consumer:\t%u\trun: %u, need: %u. ", pid, run, need);
            sem_wait(&bigBuffer->mutex);  // access the buffer
            if (bigBuffer->size > 0)  // there is something to consume
            {
                /*
                 * Data is needed and there is data in the buffer.
                 * Determine how much can be consumed.
                 */

                /*
                 * Checking borderline conditions:
                 */
                if (bigBuffer->size < bigBuffer->chunk)  // Buffer is less than 1 chunk full
                {
                    if (need <= bigBuffer->size)  // Buffer has enough to satisfy the need
                    {
                        toConsume = need;  // consume all that is needed
                        need = 0;  // consumer is satisfied
                    }
                    else  // Buffer hasn't got enough
                    {
                        toConsume = bigBuffer->size;  // consume everything from the buffer
                        need -= toConsume;  // consumer is not fully satisfied
                    }
                }
                else  // Buffer is at least 1 chunk full
                {
                    if (need <= bigBuffer->chunk)  // Need can be satisfied from 1 chunk
                    {
                        toConsume = need;  // consume all that is needed
                        need = 0;  // consumer is satisfied
                    }
                    else  // Need can't be satisfied from 1 chunk
                    {
                        toConsume = bigBuffer->chunk;  // consume everything from 1 chunk
                        need -= toConsume;  // consumer is not fully satisfied
                    }
                }

                /*
                 * Execute the consumption
                 */
                textcolour(1, RED, BLACK); printf("Consuming %u units\n", toConsume);
                for (size_t i = 0; i < toConsume; i++)
                {
                    sem_wait(&bigBuffer->semFull);
                    popFifoBig(bigBuffer);  // consume and discard
                    sem_post(&bigBuffer->semEmpty);
                }

                /*
                 * Open the mutex after consuming from the buffer.
                 * Possibly another consumer/producer will consume/produce data
                 * in the buffer
                 */
                sem_post(&bigBuffer->mutex);
                
                /* 
                if (need <= bigBuffer->size)  // buffer size is enough or more than needed
                {
                    toConsume = need;  // consume all that is needed
                    need = 0;
                    if (toConsume > bigBuffer->chunk)  // consume as much as allowed
                    {
                        textcolour(1, RED, BLACK); printf("Consuming %u units\n", bigBuffer->chunk);
                        for (size_t i = 0; i < bigBuffer->chunk; i++)
                        {
                            sem_wait(&bigBuffer->semFull);
                            popFifoBig(bigBuffer);  // consume and discard
                            sem_post(&bigBuffer->semEmpty);
                        }
                        toConsume -= bigBuffer->chunk;
                    }
                    else  // consume all that is to be consumed
                    {
                        textcolour(1, RED, BLACK); printf("Consuming %u units\n", need);
                        for (size_t i = 0; i < toConsume; i++)
                        {
                            sem_wait(&bigBuffer->semFull);
                            popFifoBig(bigBuffer);  // consume and discard
                            sem_post(&bigBuffer->semEmpty);
                        }
                        toConsume = 0;
                    }
                }
                else  // need more data than there is in the buffer
                {
                    toConsume = bigBuffer->size;  // consume entire buffer
                    need -= toConsume;  // still need more data
                }
                 */
            }
            else  // buffer is empty
            {
                sem_post(&bigBuffer->mutex);  // open mutex and wait
                textcolour(1, RED, BLACK); printf("Waiting for %u more units\n", need);
                // usleep(100000);  // 0.1s
                // usleep(USEC);
            }
        }        
    }

    textcolour(0, RED, BLACK); printf("Consumer:\t%u\tFinishing:\n", pid);
    textcolour(0, RED, BLACK); printf("Consumer:\t%u\t", pid);
    textcolour(0, RED, BLACK); printFifoBig(bigBuffer);

    return 0;
}
