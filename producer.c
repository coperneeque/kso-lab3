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
#include "textcolour.h"
#include "shared_mem.h"


#define PROD_CAP    10
#define RANGE       100

int main(int argc, char **argv)
{
    int run = 5;
    int produced;
    int products[PROD_CAP];
    int toInsert;
    int bufEmpty;
    int pid = getpid();

// attach to existing (hopefully]) big buffer
    int bigBlockId = getMemBlock(SHMEM_FILE, 0, sizeof(Fifo_big_t));
    Fifo_big_t *bigBuffer = attachMemBlock(bigBlockId);
    textcolour(0, GREEN, BLACK); printf("Producer:\t%u\tAttached to shared big buffer:\n", pid);
    textcolour(0, GREEN, BLACK); printf("Producer:\t%u\t", pid);
    textcolour(0, GREEN, BLACK); printFifoBig(bigBuffer);

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
        
        textcolour(0, GREEN, BLACK); printf("Producer:\t%u\trun: %u, produced: %u units\n", pid, run, produced);
        while (produced > 0)
        {
            textcolour(0, GREEN, BLACK); printf("Producer:\t%u\trun: %u, to insert: %u units. ", pid, run, produced);
            sem_wait(&bigBuffer->mutex);  // access the buffer
            bufEmpty = bigBuffer->capacity - bigBuffer->size;
            if (bufEmpty > 0)  // there is space in the buffer
            {
                /*
                 * There is data produced and there is space in the buffer.
                 * Determine how much can be inserted.
                 */

                /*
                 * Check borderline conditions:
                 */
                if (bufEmpty < bigBuffer->chunk)  // buffer is less than 1 chunk empty
                {
                    if (produced <= bufEmpty)  // all produced data will fit in the buffer
                    {
                        toInsert = produced;  // all produced data can be inserted
                        produced = 0;  // producer is satisfied
                    }
                    else  // only some of the produced data will fit in the buffer
                    {
                        toInsert = bufEmpty;  // can insert to fill up the buffer
                        produced -= toInsert;  // producer is not satisfied
                    }
                }
                else  // buffer is at least 1 chunk empty
                {
                    if (produced <= bigBuffer->chunk)  // all produced data will fit in 1 chunk
                    {
                        toInsert = produced;  // all produced data can be inserted
                        produced = 0;  // producer is satisfied
                    }
                    else  // produced data will not fit in 1 chunk
                    {
                        toInsert = bigBuffer->chunk;  // some of the data can be inserted into 1 full chunk
                        produced -= toInsert;  // producer is not satisfied
                    }
                }

                /*
                 * Execute the insertion:
                 */
                textcolour(1, GREEN, BLACK); printf("Inserting %u units\n", toInsert);
                for (size_t i = 0; i < toInsert; i++)
                {
                    sem_wait(&bigBuffer->semEmpty);
                    putFifoBig(bigBuffer,products[i]);
                    sem_post(&bigBuffer->semFull);
                }

                /*
                 * Open the mutex after inserting into the buffer.
                 * Possibly another consumer/producer will consume/produce data
                 */
                sem_post(&bigBuffer->mutex);
            }
            else  // no space in the buffer
            {
                sem_post(&bigBuffer->mutex);
                /*
                 * If there is no space in the buffer then the producer can't wait.
                 * In normal conditions running out of buffer space results in data loss.
                 * This is simulated here by resetting of the produced variable.
                 */
                produced = 0;
                textcolour(0, GREEN, BLACK); printf("Producer:\t%u\tNo space in buffer - dropping data.\n", pid);
            }
            
            

          /*   if (produced <= bigBuffer->capacity - bigBuffer->size)  // is there space in the buffer
            {
                if (produced > bigBuffer->chunk)
                {
                    textcolour(1, GREEN, BLACK); printf("Inserting %u units\n", bigBuffer->chunk);
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
                    textcolour(1, GREEN, BLACK); printf("Inserting %u units\n", produced);
                    for (size_t i = 0; i < produced; i++)
                    {
                        sem_wait(&bigBuffer->semEmpty);
                        putFifoBig(bigBuffer,products[i]);
                        sem_post(&bigBuffer->semFull);
                    }
                    produced = 0;
                }
            }
            else  // no space in the buffer
            {
                textcolour(0, GREEN, BLACK); printf("Producer:\t%u\trun: %u, to insert: %u units. ", pid, run, produced);
            } */
            // sem_post(&bigBuffer->mutex);
            // sleep(1);
        }        
    }

    textcolour(0, GREEN, BLACK); printf("Producer:\t%u\tFinishing:\n", pid);
    textcolour(0, GREEN, BLACK); printf("Producer:\t%u\t", pid);
    textcolour(0, GREEN, BLACK); printFifoBig(bigBuffer);

    return 0;
}
