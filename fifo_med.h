/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021-z                                                           *
 * lab3                                                                 *
 ************************************************************************/
#ifndef FIFO_MED_H
#define FIFO_MED_H

#define FIFO_MED_CAPACITY 60
#define FIFO_MED_CHUNK 5

typedef struct
{
    sem_t       mutex;
    sem_t       semFull;
    sem_t       semEmpty;
    int         data[FIFO_MED_CAPACITY];
    unsigned    head_idx;
    unsigned    tail_idx;
    unsigned    capacity;
    unsigned    size;
    unsigned    chunk;
} Fifo_med_t;

/*
 * Set initial values
 */
void initFifoMed();

void putFifoMed(Fifo_med_t*, int);
int popFifoMed(Fifo_med_t*);

/*
 * Empty the Fifo by resetting head and tail
 */
void flushFifoMed(Fifo_med_t*);

/*
 * Populate a random precentage between 30% and 70%
 * with random values
 */
void randFillFifoMed(Fifo_med_t*);

#endif
