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
#include "test_flags.h"
#include "textcolour.h"


#define PROD_CAP    10
#define RANGE       100
#define USEC        100000
#define WAIT_CAP    1000000  // 1s

int main(int argc, char **argv)
{
    int run = 40;
    int produced;
    int products[PROD_CAP];
    int toInsert;
    int bufEmpty;
    int pid = getpid();
    long totalWait = 0;

// attach to existing (hopefully]) medium buffer
    int medBlockId = getMemBlock(SHMEM_FILE, 1, sizeof(Fifo_med_t));
    Fifo_med_t *medBuffer = attachMemBlock(medBlockId);
        #ifdef MP_V_VERBOSE
    textcolour(0, BLUE, BLACK); printf("Producer B:\t%u\tAttached to shared medium buffer:\n", pid);
    textcolour(0, BLUE, BLACK); printf("Producer B:\t%u\t", pid);
    textcolour(0, BLUE, BLACK); printFifoMed(medBuffer);
        #endif

    // srandom(time(NULL));

    while(run) {
        // run = random() % 30;
        --run;
        produced = random() % PROD_CAP;  // how much is produced
        for (size_t i = 0; i < produced; i++) {  // simulate production
            products[i] = random() % RANGE;
        }
        
            #ifdef MP_VERBOSE
        textcolour(0, BLUE, BLACK); printf("Producer B:\t%u\trun: %u, produced: %u units\n", pid, run, produced);
            #endif
        while (produced > 0) {
                #ifdef MP_VERBOSE
            textcolour(0, BLUE, BLACK); printf("Producer B:\t%u\trun: %u, to insert: %u units. ", pid, run, produced);
                #endif
            sem_wait(&medBuffer->mutex);  // access the buffer
            bufEmpty = medBuffer->capacity - medBuffer->size;
            if (bufEmpty > 0) {  // there is space in the buffer
                /*
                 * There is data produced and there is space in the buffer.
                 * Determine how much can be inserted.
                 */
                /*
                 * Check borderline conditions:
                 */
                if (bufEmpty < medBuffer->chunk) {  // buffer is less than 1 chunk empty
                    if (produced <= bufEmpty) {  // all produced data will fit in the buffer
                        toInsert = produced;  // all produced data can be inserted
                        produced = 0;  // producer is satisfied
                    } else {  // only some of the produced data will fit in the buffer
                        toInsert = bufEmpty;  // can insert to fill up the buffer
                        produced -= toInsert;  // producer is not satisfied
                    }
                } else {  // buffer is at least 1 chunk empty
                    if (produced <= medBuffer->chunk) {  // all produced data will fit in 1 chunk
                        toInsert = produced;  // all produced data can be inserted
                        produced = 0;  // producer is satisfied
                    } else {  // produced data will not fit in 1 chunk
                        toInsert = medBuffer->chunk;  // some of the data can be inserted into 1 full chunk
                        produced -= toInsert;  // producer is not satisfied
                    }
                }
                /*
                 * Execute the insertion:
                 */
                    #ifdef MP_VERBOSE
                textcolour(1, BLUE, BLACK); printf("Inserting %u units\n", toInsert);
                    #endif
                for (size_t i = 0; i < toInsert; i++) {
                    sem_wait(&medBuffer->semEmpty);
                    putFifoMed(medBuffer,products[i]);
                    sem_post(&medBuffer->semFull);
                }
                /*
                 * Open the mutex after inserting into the buffer.
                 * Possibly another consumer/producer will consume/produce data
                 */
                sem_post(&medBuffer->mutex);
            } else {  // no space in the buffer
                sem_post(&medBuffer->mutex);
                /*
                 * If there is no space in the buffer then the producer can't wait.
                 * In normal conditions running out of buffer space results in data loss.
                 * This is simulated here by zeroing of the 'produced' variable.
                 */
                produced = 0;
                    #ifdef MP_VERBOSE
                textcolour(1, BLUE, BLACK); printf("No space in buffer - dropping data.\n", pid);
                    #endif
                // usleep(USEC);
                totalWait += USEC;
                if (totalWait > WAIT_CAP) {
                    produced = 0;
                    run = 0;
                    textcolour(0, BLUE, BLACK); printf("Producer B:\t%u\tWaiting timed-out - exiting.\n", pid);
                }
            } 
        }        
    }

        #ifdef MP_V_VERBOSE
    textcolour(0, BLUE, BLACK); printf("Producer B:\t%u\tFinishing:\n", pid);
    textcolour(0, BLUE, BLACK); printf("Producer B:\t%u\t", pid);
    textcolour(0, BLUE, BLACK); printFifoMed(medBuffer);
        #endif

    return 0;
}