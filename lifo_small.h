/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#ifndef LIFO_SMALL_H
#define LIFO_SMALL_H

#define LIFO_SMALL_CAPACITY 30
#define LIFO_SMALL_CHUNK 4

typedef struct
{
    int arr[LIFO_SMALL_CAPACITY];
    unsigned head_idx;
    unsigned capacity;
    unsigned size;
    unsigned chunk;
} Lifo_small_t;

void initLifoSmall();
void putLifoSmall(Lifo_small_t*, int);
int popLifoSmall(Lifo_small_t*);

#endif
