/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#ifndef FIFO_BIG_H
#define FIFO_BIG_H

#define FIFO_BIG_CAPACITY 100
#define FIFO_BIG_CHUNK 3

typedef struct
{
    int arr[FIFO_BIG_CAPACITY];
    unsigned head_idx;
    unsigned tail_idx;
    unsigned capacity;
    unsigned size;
    unsigned chunk;
} Fifo_big_t;

void initFifoBig();
void putFifoBig(Fifo_big_t*, int);
int popFifoBig(Fifo_big_t*);

#endif
