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
    int data[LIFO_SMALL_CAPACITY];
    unsigned head_idx;
    unsigned capacity;
    unsigned size;
    unsigned chunk;
} Lifo_small_t;

/*
 * Set initial values
 */
void initLifoSmall();

void putLifoSmall(Lifo_small_t*, int);
int popLifoSmall(Lifo_small_t*);

/*
 * Empty the Lifo by resetting head
 */
void flushLifoSmall(Lifo_small_t*);

/*
 * Populate a random precentage between 30% and 70%
 * with random values
 */
void randFillLifoSmall(Lifo_small_t*);
#endif
