/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021-z                                                           *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#include "lifo_small.h"
#include "simple_test.h"
#include "test_flags.h"


extern int errno;

void initLifoSmall(Lifo_small_t *l)
{
    errno = 0;

    if (l == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG        
        perror("initLifoSmall(): null pointer");
#endif
        return;
    }

    l->capacity = LIFO_SMALL_CAPACITY;
    l->size     = 0;
    l->head_idx = 0;
    l->chunk    = LIFO_SMALL_CHUNK;
    sem_init(&l->mutex, 1, 1);
    sem_init(&l->semEmpty, 1, LIFO_SMALL_CAPACITY);
    sem_init(&l->semFull, 1, 0);
}

void putLifoSmall(Lifo_small_t *l, int k)
{
    errno = 0;

    if (l == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG        
        perror("putLifoSmall(): NULL pointer");
#endif
        return;
    }

    if (l->size == l->capacity)
    {
        errno = ENOBUFS;
#ifdef MP_DEBUG        
        perror("putLifoSmall(): Buffer is full");
#endif
        return;
    }

    l->data[l->head_idx] = k;
    ++l->head_idx;
    ++(l->size);
#ifdef MP_DEBUG
    printf("putLifoSmall(): head_idx: %u, k: %d, size: %u\n", l->head_idx, k, l->size);
#endif
}

int popLifoSmall(Lifo_small_t* l)
{
    errno = 0;

    if (l == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("popLifoSmall(): null pointer.");
#endif
        return -1;
    }

    if (l->size == 0)
    {
        errno = ENODATA;
#ifdef MP_DEBUG        
        perror("popLifoSmall(): size is 0.");
#endif
        return -1;
    }

    --l->head_idx;
    int ret = l->data[l->head_idx];
    --l->size;
#ifdef MP_DEBUG
    printf("popLifoSmall(): head_idx: %u, ret: %d, size: %u\n", l->head_idx, ret, l->size);
#endif

    return ret;
}

void flushLifoSmall(Lifo_small_t *l)
{
    errno = 0;

    if (l == NULL)
    {
        errno = EINVAL;
#ifdef MP_DEBUG
        perror("flushLifoSmall(): null pointer");
#endif
        return;
    }

    l->size     = 0;
    l->head_idx = 0;
}

void randFillLifoSmall(Lifo_small_t* l)
{
#define LOWER 30
#define UPPER 70
#define RANGE 100
        
    errno = 0;

    if (l == NULL)
    {
        errno = EINVAL;
            #ifdef MP_DEBUG
        perror("randFillLifoSmall(): null pointer");
            #endif
        return;
    }

    flushLifoSmall(l);

    srandom(time(NULL));
    unsigned percentage = LOWER + random() % (UPPER - LOWER);
    double bound = (double)percentage / 100 * l->capacity;

    for (size_t i = 0; i < (size_t)bound; i++)
    {
        putLifoSmall(l, random() % RANGE);
    }
}
