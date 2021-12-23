/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "lifo_small.h"
#include "simple_test.h"
#include "test_flags.h"

void initLifoSmall(Lifo_small_t *l)
{
    extern int errno;
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
}

void putLifoSmall(Lifo_small_t *l, int k)
{
    extern int errno;
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

    l->arr[l->head_idx] = k;
    ++l->head_idx;
    ++(l->size);
#ifdef MP_DEBUG
    printf("putLifoSmall(): head_idx: %u, k: %d, size: %u\n", l->head_idx, k, l->size);
#endif
}

int popLifoSmall(Lifo_small_t* l)
{
    extern int errno;
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
    int ret = l->arr[l->head_idx];
    --l->size;
#ifdef MP_DEBUG
    printf("popLifoSmall(): head_idx: %u, ret: %d, size: %u\n", l->head_idx, ret, l->size);
#endif

    return ret;
}
