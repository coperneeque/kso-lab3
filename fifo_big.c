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

#include "fifo_big.h"
#include "simple_test.h"
#include "test_flags.h"


extern int errno;

void initFifoBig(Fifo_big_t *f)
{
    errno = 0;

    if (f == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("initFifoBig(): null pointer");
#endif
        return;
    }

    f->capacity = FIFO_BIG_CAPACITY;
    f->size     = 0;
    f->head_idx = 0;
    f->tail_idx = 0;
    f->chunk    = FIFO_BIG_CHUNK;
    sem_init(&f->mutex, 1, 1);
    sem_init(&f->semEmpty, 1, FIFO_BIG_CAPACITY);
    sem_init(&f->semFull, 1, 0);
}

void putFifoBig(Fifo_big_t *f, int k)
{
    errno = 0;

    if (f == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("putFifoBig(): NULL pointer");
#endif
        return;
    }

    if (f->size == f->capacity)
    {
        errno = ENOBUFS;
#ifdef MP_DEBUG
        perror("putFifoBig(): Buffer is full");
#endif
        return;
    }

    if (f->size == 0)
    {
        f->data[f->head_idx] = k;
        ++(f->size);
#ifdef MP_DEBUG
        printf("putFifoBig(): head_idx: %u, k: %d, size: %u\n", f->head_idx, k, f->size);        
#endif
        return;
    }

    // size > 0    
    f->head_idx = (f->head_idx + 1 == f->capacity ? 0 : f->head_idx + 1);  // head_idx = head_idx+1 mod 100
    f->data[f->head_idx] = k;
    ++(f->size);
#ifdef MP_DEBUG
    printf("putFifoBig(): head_idx: %u, k: %d, size: %u\n", f->head_idx, k, f->size);
#endif
}

int popFifoBig(Fifo_big_t* f)
{
    errno = 0;

    if(f == NULL) 
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("popFifoBig(): null pointer.");
#endif
        return -1;
    }

    if (f->size == 0)
    {
        errno = ENODATA;
#ifdef MP_DEBUG
        perror("popFifoBig(): size is 0.");
#endif
        return -1;
    }

    if (f->size == 1)
    {
        --f->size;
        int ret = f->data[f->tail_idx];
#ifdef MP_DEBUG
        printf("popFifoBig(): tail_idx: %u, ret: %d, size: %u\n", f->tail_idx, ret, f->size);
#endif
        return ret;
    }
    
    int ret = f->data[f->tail_idx];
    f->tail_idx = (f->tail_idx + 1 == f->capacity ? 0 : f->tail_idx + 1);  // tail_idx = tail_idx+1 mod 10
    --f->size;
#ifdef MP_DEBUG
    printf("popFifoBig(): tail_idx: %u, ret: %d, size: %u\n", f->tail_idx, ret, f->size);
#endif
    return ret;
}

void printFifoBig(Fifo_big_t* f)
{
    printf("Big FIFO buffer. Capacity: %u, size: %u, tail_idx: %u, head_idx: %u, chunk: %u\n", f->capacity, f->size, f->tail_idx, f->head_idx, f->chunk);
}

void flushFifoBig(Fifo_big_t *f)
{
    errno = 0;

    if (f == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("initFifoBig(): null pointer");
#endif
        return;
    }

    f->size     = 0;
    f->head_idx = 0;
    f->tail_idx = 0;
    sem_init(&f->semEmpty, 1, FIFO_BIG_CAPACITY);
    sem_init(&f->semFull, 1, 0);
}

void randFillFifoBig(Fifo_big_t* f)
{
#define LOWER 30
#define UPPER 70
#define RANGE 100
        
    errno = 0;

    if (f == NULL)
    {
        errno = EINVAL;
            #ifdef MP_DEBUG
        perror("randFillFifoBig(): null pointer");
            #endif
        return;
    }

    flushFifoBig(f);

    srandom(time(NULL));
    unsigned percentage = LOWER + random() % (UPPER - LOWER);
    double bound = (double)percentage / 100 * f->capacity;

    for (size_t i = 0; i < (size_t)bound; i++)
    {
        putFifoBig(f, random() % RANGE);
    }
    sem_init(&f->semEmpty, 1, FIFO_BIG_CAPACITY - (unsigned)bound);
    sem_init(&f->semFull, 1, (unsigned)bound);
}
