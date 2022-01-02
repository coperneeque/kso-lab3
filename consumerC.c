/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021-z                                                           *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#include "lifo_small.h"
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

// attach to existing (hopefully]) small buffer
    int smallBlockId = getMemBlock(SHMEM_FILE, 2, sizeof(Lifo_small_t));
    Lifo_small_t *smallBuffer = attachMemBlock(smallBlockId);
        #ifdef MP_V_VERBOSE
    textcolour(0, MAGENTA, BG_BLACK); printf("Consumer C:\t%u\tAttached to shared small buffer:\n", pid);
    textcolour(0, MAGENTA, BG_BLACK); printf("Consumer C:\t%u\t", pid);
    textcolour(0, MAGENTA, BG_BLACK); printLifoSmall(smallBuffer);
        #endif

    // srandom(time(NULL));

    while(run)
    {
        // run = random() % 30;
        --run;
        need = random() % NEED_CAP;  // how much data needed by consumer
            #ifdef MP_VERBOSE
        textcolour(0, MAGENTA, BG_BLACK); printf("Consumer C:\t%u\trun: %u, need: %u. \n", pid, run, need);
            #endif
        while (need > 0)
        {
                #ifdef MP_VERBOSE
            textcolour(0, MAGENTA, BG_BLACK); printf("Consumer C:\t%u\trun: %u, need: %u. ", pid, run, need);
                #endif
            sem_wait(&smallBuffer->mutex);  // access the buffer
            if (smallBuffer->size > 0)  // there is something to consume
            {
                /*
                 * Data is needed and there is data in the buffer.
                 * Determine how much can be consumed.
                 */
                /*
                 * Checking borderline conditions:
                 */
                if (smallBuffer->size < smallBuffer->chunk)  // Buffer is less than 1 chunk full
                {
                    if (need <= smallBuffer->size)  // Buffer has enough to satisfy the need
                    {
                        toConsume = need;  // consume all that is needed
                        need = 0;  // consumer is satisfied
                    }
                    else  // Buffer hasn't got enough
                    {
                        toConsume = smallBuffer->size;  // consume everything from the buffer
                        need -= toConsume;  // consumer is not fully satisfied
                    }
                }
                else  // Buffer is at least 1 chunk full
                {
                    if (need <= smallBuffer->chunk)  // Need can be satisfied from 1 chunk
                    {
                        toConsume = need;  // consume all that is needed
                        need = 0;  // consumer is satisfied
                    }
                    else  // Need can't be satisfied from 1 chunk
                    {
                        toConsume = smallBuffer->chunk;  // consume everything from 1 chunk
                        need -= toConsume;  // consumer is not fully satisfied
                    }
                }
                /*
                 * Execute the consumption
                 */
                    #ifdef MP_VERBOSE
                textcolour(UNDERLINE, MAGENTA, BG_BLACK); printf("Consuming %u units\n", toConsume);
                    #endif
                for (size_t i = 0; i < toConsume; i++)
                {
                    sem_wait(&smallBuffer->semFull);
                    popLifoSmall(smallBuffer);  // consume and discard
                    sem_post(&smallBuffer->semEmpty);
                }
                /*
                 * Open the mutex after consuming from the buffer.
                 * Possibly another consumer/producer will consume/produce data
                 * in the buffer
                 */
                sem_post(&smallBuffer->mutex);
            }
            else  // buffer is empty
            {
                sem_post(&smallBuffer->mutex);  // open mutex and wait
                    #ifdef MP_VERBOSE
                textcolour(UNDERLINE, MAGENTA, BG_BLACK); printf("Waiting for %u more units\n", need);
                    #endif
                // usleep(USEC);
                totalWait += USEC;
                if (totalWait > WAIT_CAP)
                {
                    need = 0;
                    run = 0;
                    textcolour(0, MAGENTA, BG_BLACK); printf("Consumer C:\t%u\tWaiting timed-out - exiting.\n", pid);
                }
                
            }
        }        
    }

        #ifdef MP_V_VERBOSE
    textcolour(0, MAGENTA, BG_BLACK); printf("Consumer C:\t%u\tFinishing:\n", pid);
    textcolour(0, MAGENTA, BG_BLACK); printf("Consumer C:\t%u\t", pid);
    textcolour(0, MAGENTA, BG_BLACK); printLifoSmall(smallBuffer);
        #endif
    shmdt(smallBuffer);

    return 0;
}
