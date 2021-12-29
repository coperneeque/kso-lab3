/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#ifndef FIFO_BIG_H
#define FIFO_BIG_H

#include <semaphore.h>

#define FIFO_BIG_CAPACITY 100
#define FIFO_BIG_CHUNK 3

typedef struct
{
    sem_t       mutex;
    sem_t       semFull;
    sem_t       semEmpty;
    int         data[FIFO_BIG_CAPACITY];
    unsigned    head_idx;
    unsigned    tail_idx;
    unsigned    capacity;
    unsigned    size;
    unsigned    chunk;
} Fifo_big_t;

/*
 * Set initial values
 */
void initFifoBig(Fifo_big_t*);

void putFifoBig(Fifo_big_t*, int);
int popFifoBig(Fifo_big_t*);
void printFifoBig(Fifo_big_t*);

/*
 * Empty the Fifo by resetting head and tail
 */
void flushFifoBig(Fifo_big_t*);

/*
 * Populate a random precentage between 30% and 70%
 * with random values
 */
void randFillFifoBig(Fifo_big_t*);

#endif
