/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#include "fifo_med.h"
#include "simple_test.h"
#include "test_flags.h"


extern int errno;

void initFifoMed(Fifo_med_t *f)
{
    errno = 0;

    if (f == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("initFifoMed(): null pointer");
#endif
        return;
    }

    f->capacity = FIFO_MED_CAPACITY;
    f->size     = 0;
    f->head_idx = 0;
    f->tail_idx = 0;
    f->chunk    = FIFO_MED_CHUNK;
    sem_init(&f->mutex, 1, 1);
    sem_init(&f->semEmpty, 1, FIFO_MED_CAPACITY);
    sem_init(&f->semFull, 1, 0);
}

void putFifoMed(Fifo_med_t *f, int k)
{
    errno = 0;

    if (f == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG        
        perror("putFifoMed(): NULL pointer");
#endif
        return;
    }

    if (f->size == f->capacity)
    {
        errno = ENOBUFS;
#ifdef MP_DEBUG        
        perror("putFifoMed(): Buffer is full");
#endif
        return;
    }

    if (f->size == 0)
    {
        f->data[f->head_idx] = k;
        ++(f->size);
#ifdef MP_DEBUG
        printf("putFifoMed(): head_idx: %u, k: %d, size: %u\n", f->head_idx, k, f->size);        
#endif
        return;
    }
        
    f->head_idx = (f->head_idx + 1 == f->capacity ? 0 : f->head_idx + 1);  // head_idx = head_idx+1 mod 60
    f->data[f->head_idx] = k;
    ++(f->size);
#ifdef MP_DEBUG
    printf("putFifoMed(): head_idx: %u, k: %d, size: %u\n", f->head_idx, k, f->size);
#endif
}

int popFifoMed(Fifo_med_t* f)
{
    errno = 0;

    if(f == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("popFifoMed(): null pointer.");
#endif
        return -1;
    }

    if (f->size == 0)
    {
        errno = ENODATA;
#ifdef MP_DEBUG        
        perror("popFifoMed(): size is 0.");
#endif
        return -1;
    }

    if (f->size == 1)
    {
        int ret = f->data[f->tail_idx];
        --f->size;
#ifdef MP_DEBUG
        printf("popFifoMed(): tail_idx: %u, ret: %d, size: %u\n", f->tail_idx, ret, f->size);
#endif
        return ret;
    }
    
    int ret = f->data[f->tail_idx];
    f->tail_idx = (f->tail_idx + 1 == f->capacity ? 0 : f->tail_idx + 1);  // tail_idx = tail_idx+1 mod 60
    --f->size;
#ifdef MP_DEBUG
    printf("popFifoMed(): tail_idx: %u, ret: %d, size: %u\n", f->tail_idx, ret, f->size);
#endif

    return ret;
}

void flushFifoMed(Fifo_med_t *f)
{
    errno = 0;

    if (f == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("flushFifoMed(): null pointer");
#endif
        return;
    }

    f->size     = 0;
    f->head_idx = 0;
    f->tail_idx = 0;
}

void randFillFifoMed(Fifo_med_t* f)
{
#define LOWER 30
#define UPPER 70
#define RANGE 100
        
    errno = 0;

    if (f == NULL)
    {
        errno = EINVAL;
            #ifdef MP_DEBUG
        perror("randFillFifoMed(): null pointer");
            #endif
        return;
    }

    flushFifoMed(f);

    srandom(time(NULL));
    unsigned percentage = LOWER + random() % (UPPER - LOWER);
    double bound = (double)percentage / 100 * f->capacity;

    for (size_t i = 0; i < (size_t)bound; i++)
    {
        putFifoMed(f, random() % RANGE);
    }
}
