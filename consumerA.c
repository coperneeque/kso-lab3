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
// #include "fifo_med.h"
// #include "lifo_small.h"
#include "shared_mem.h"
#include "test_flags.h"
#include "textcolour.h"


#define NEED_CAP    10
#define USEC        100000
#define WAIT_CAP    1000000  // 30s

int main(int argc, char **argv)
{
    int run = 40;
    int need;
    int toConsume;
    int pid = getpid();
    long totalWait = 0;

// attach to existing (hopefully]) big buffer
    int bigBlockId = getMemBlock(SHMEM_FILE, 0, sizeof(Fifo_big_t));
    Fifo_big_t *bigBuffer = attachMemBlock(bigBlockId);
        #ifdef MP_V_VERBOSE
    textcolour(0, RED, BG_BLACK); printf("Consumer A:\t%u\tAttached to shared big buffer:\n", pid);
    textcolour(0, RED, BG_BLACK); printf("Consumer A:\t%u\t", pid);
    textcolour(0, RED, BG_BLACK); printFifoBig(bigBuffer);
        #endif

    // srandom(time(NULL));

    while(run)
    {
        // run = random() % 30;
        --run;
        need = random() % NEED_CAP;  // how much data needed by consumer
            #ifdef MP_VERBOSE
        textcolour(0, RED, BG_BLACK); printf("Consumer A:\t%u\trun: %u, need: %u. \n", pid, run, need);
            #endif
        while (need > 0)
        {
                #ifdef MP_VERBOSE
            textcolour(0, RED, BG_BLACK); printf("Consumer A:\t%u\trun: %u, need: %u. ", pid, run, need);
                #endif
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
                    #ifdef MP_VERBOSE
                textcolour(4, RED, BG_BLACK); printf("Consuming %u units\n", toConsume);
                    #endif
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
            }
            else  // buffer is empty
            {
                sem_post(&bigBuffer->mutex);  // open mutex and wait
                    #ifdef MP_VERBOSE
                textcolour(4, RED, BG_BLACK); printf("Waiting for %u more units\n", need);
                    #endif
                // usleep(USEC);
                totalWait += USEC;
                if (totalWait > WAIT_CAP)
                {
                    need = 0;
                    run = 0;
                    textcolour(0, RED, BG_BLACK); printf("Consumer A:\t%u\tWaiting timed-out - exiting.\n", pid);
                }
                
            }
        }        
    }

        #ifdef MP_V_VERBOSE
    textcolour(0, RED, BG_BLACK); printf("Consumer A:\t%u\tFinishing:\n", pid);
    textcolour(0, RED, BG_BLACK); printf("Consumer A:\t%u\t", pid);
    textcolour(0, RED, BG_BLACK); printFifoBig(bigBuffer);
        #endif

    return 0;
}
