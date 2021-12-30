/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021-z                                                           *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "fifo_big.h"
#include "fifo_med.h"
#include "lifo_small.h"
#include "shared_mem.h"
#include "textcolour.h"
 
#include "simple_test.h"
#include "test_flags.h"


Fifo_big_t *getBigBuffer(int id);
Fifo_med_t *getMedBuffer(int id);
Lifo_small_t *getSmallBuffer(int id);

int main(int argc, char **argv)
{
// create big buffer
    int bigBlockId = getMemBlock(SHMEM_FILE, 0, sizeof(Fifo_big_t));
    Fifo_big_t *bigBuffer = getBigBuffer(bigBlockId);
    // randFillFifoBig(bigBuffer);

// create medium buffer
    int medBlockId = getMemBlock(SHMEM_FILE, 1, sizeof(Fifo_med_t));
    Fifo_med_t *medBuffer = getMedBuffer(medBlockId);

// create small buffer
    int smallBlockId = getMemBlock(SHMEM_FILE, 2, sizeof(Lifo_small_t));
    Lifo_small_t *smallBuffer = getSmallBuffer(smallBlockId);

    pid_t parentpid = getpid();
    srandom(time(NULL));

    if (fork() == 0) {  // this is child process:
            #ifdef MP_V_VERBOSE
        printf("Child:\t\tParent executed fork(), child pid: %u. Executing consumer process - execv(\"./consumer\", NULL)\n", getpid());
            #endif
        execv("./consumer", NULL);
    }
    else {  // parent process:
        if (fork() == 0) {  // parent spawning 2nd child:
                #ifdef MP_V_VERBOSE
            textcolour(0, WHITE, BLACK);
            printf("Child:\t\tParent executed fork(), child pid: %u. Executing producer process - execv(\"./producer\", NULL)\n", getpid());
                #endif
            execv("./producer", NULL);
        }
        else if (fork() == 0)  // parent spawning 3rd child:
        {
            // printf("Child:\t\tParent spawned child with pid: %u\n", getpid());
            // printf("waiting on sem... ");
            // sem_wait(&smallBuffer->semEmpty);
            // printf("done\n");
        }
    }

    if (getpid() == parentpid) {
        while(wait(NULL) > 0);  // wait for all consumers/producers to finish
            #ifdef MP_V_VERBOSE
        textcolour(0, WHITE, BLACK);
        printf("Parent:\t\tExiting:\n");
        printf("Parent:\t\t");
        printFifoBig(bigBuffer);
            #endif
        shmdt(bigBuffer);
        shmdt(medBuffer);
        shmdt(smallBuffer);
        shmctl(bigBlockId, IPC_RMID, NULL);
        shmctl(medBlockId, IPC_RMID, NULL);
        shmctl(smallBlockId, IPC_RMID, NULL);
    }
    else {  // all children exiting
        // printf("child pid: %u exiting\n", getpid());
    }
    
    return 0;
}

Fifo_big_t *getBigBuffer(int id)
{
    Fifo_big_t *buf = attachMemBlock(id);
#ifdef TEST_FIFO_BIG
    test_FifoBig(buf);
#endif
#ifndef TEST_FIFO_BIG
    initFifoBig(buf);
#endif
    // printf("bigBuffer before fork(): capacity: %u, size: %u\n", bigBuffer->capacity, bigBuffer->size);

    return buf;
}

Fifo_med_t *getMedBuffer(int id)
{
    Fifo_med_t *buf = attachMemBlock(id);
#ifdef TEST_FIFO_MED
    test_FifoMed(buf);
#endif
#ifndef TEST_FIFO_MED
    initFifoMed(buf);
#endif
    // printf("medBuffer before fork(): capacity: %u, size: %u\n", medBuffer->capacity, medBuffer->size);
    return buf;
}

Lifo_small_t *getSmallBuffer(int id)
{
    Lifo_small_t *buf = attachMemBlock(id);
#ifdef TEST_LIFO_SMALL
    test_LifoSmall(buf);
#endif
#ifndef TEST_LIFO_SMALL
    initLifoSmall(buf);
#endif
    return buf;
}
