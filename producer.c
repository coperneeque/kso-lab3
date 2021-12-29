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
    int produced;
    int pid = getpid();

// attach to existing (hopefully]) big buffer
    int bigBlockId = getMemBlock(SHMEM_FILE, 0, sizeof(Fifo_big_t));
    Fifo_big_t *bigBuffer = attachMemBlock(bigBlockId);
    printf("Producer\t%u\tAttached to shared big buffer:\n", pid);
    printFifoBig(bigBuffer);

    srandom(time(NULL));

    while(run) {
        // run = random() % 30;
        --run;
        produced = random() % 10;  // how much is produced
        printf("Producer:\t%u\trun: %u, produced: %u\n", pid, run, produced);
        
        sleep(1);
    }

    return 0;
}
