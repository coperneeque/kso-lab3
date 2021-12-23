/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "fifo_med.h"
#include "simple_test.h"
#include "test_flags.h"

void initFifoMed(Fifo_med_t *f)
{
    extern int errno;
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
}

void putFifoMed(Fifo_med_t *f, int k)
{
    extern int errno;
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
        f->arr[f->head_idx] = k;
        ++(f->size);
#ifdef MP_DEBUG
        printf("putFifoMed(): head_idx: %u, k: %d, size: %u\n", f->head_idx, k, f->size);        
#endif
        return;
    }
        
    f->head_idx = (f->head_idx + 1 == f->capacity ? 0 : f->head_idx + 1);  // head_idx = head_idx+1 mod 60
    f->arr[f->head_idx] = k;
    ++(f->size);
#ifdef MP_DEBUG
    printf("putFifoMed(): head_idx: %u, k: %d, size: %u\n", f->head_idx, k, f->size);
#endif
}

int popFifoMed(Fifo_med_t* f)
{
    extern int errno;
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
        int ret = f->arr[f->tail_idx];
        --f->size;
#ifdef MP_DEBUG
        printf("popFifoMed(): tail_idx: %u, ret: %d, size: %u\n", f->tail_idx, ret, f->size);
#endif
        return ret;
    }
    
    int ret = f->arr[f->tail_idx];
    f->tail_idx = (f->tail_idx + 1 == f->capacity ? 0 : f->tail_idx + 1);  // tail_idx = tail_idx+1 mod 60
    --f->size;
#ifdef MP_DEBUG
    printf("popFifoMed(): tail_idx: %u, ret: %d, size: %u\n", f->tail_idx, ret, f->size);
#endif

    return ret;
}

// ================   DEBUG   ================
//#undef MP_DEBUG
