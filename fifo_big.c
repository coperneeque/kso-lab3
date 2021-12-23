/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "fifo_big.h"
#include "simple_test.h"
#include "test_flags.h"

void initFifoBig(Fifo_big_t *f)
{
    extern int errno;
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
}

void putFifoBig(Fifo_big_t *f, int k)
{
    extern int errno;
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
        f->arr[f->head_idx] = k;
        ++(f->size);
#ifdef MP_DEBUG
        printf("putFifoBig(): head_idx: %u, k: %d, size: %u\n", f->head_idx, k, f->size);        
#endif
        return;
    }

    // size > 0    
    f->head_idx = (f->head_idx + 1 == f->capacity ? 0 : f->head_idx + 1);  // head_idx = head_idx+1 mod 100
    f->arr[f->head_idx] = k;
    ++(f->size);
#ifdef MP_DEBUG
    printf("putFifoBig(): head_idx: %u, k: %d, size: %u\n", f->head_idx, k, f->size);
#endif
}

int popFifoBig(Fifo_big_t* f)
{
    extern int errno;
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
        int ret = f->arr[f->tail_idx];
#ifdef MP_DEBUG
        printf("popFifoBig(): tail_idx: %u, ret: %d, size: %u\n", f->tail_idx, ret, f->size);
#endif
        return ret;
    }
    
    int ret = f->arr[f->tail_idx];
    f->tail_idx = (f->tail_idx + 1 == f->capacity ? 0 : f->tail_idx + 1);  // tail_idx = tail_idx+1 mod 10
    --f->size;
#ifdef MP_DEBUG
    printf("popFifoBig(): tail_idx: %u, ret: %d, size: %u\n", f->tail_idx, ret, f->size);
#endif
    return ret;
}
