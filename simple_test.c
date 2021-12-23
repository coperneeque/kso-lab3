/************************************************************************
 * Mikolaj Panka                                                        *
 * KSO 2021                                                             *
 * lab3                                                                 *
 ************************************************************************/
#include "simple_test.h"

#include <stdio.h>

#include "fifo_big.h"
#include "fifo_med.h"
#include "lifo_small.h"


void test_FifoBig()
{
    Fifo_big_t fifo;
    initFifoBig(&fifo);
    
    for (size_t i = 0; i < 10; i++)
    {
        putFifoBig(&fifo, 21 + i);
    }
    
    for (size_t i = 0; i < 10; i++)
    {
        popFifoBig(&fifo);
    }

    popFifoBig(&fifo);
    popFifoBig(&fifo);

    for (size_t i = 0; i < 5; i++)
    {
        putFifoBig(&fifo, 21 + i);
    }

    for (size_t i = 0; i < 6; i++)
    {
        popFifoBig(&fifo);
    }

    printf("[  FifoBig  ]   head_idx: %u, tail_idx: %u, capacity: %u, size: %u\n", fifo.head_idx, fifo.tail_idx, fifo.capacity, fifo.size);
    
    for (size_t i = 0; i < 5; i++)
    {
        putFifoBig(&fifo, 21 + i);
    }

    for (size_t i = 0; i < 6; i++)
    {
        popFifoBig(&fifo);
    }

    printf("[  FifoBig  ]   head_idx: %u, tail_idx: %u, capacity: %u, size: %u\n", fifo.head_idx, fifo.tail_idx, fifo.capacity, fifo.size);
}

void test_FifoMed()
{
    Fifo_med_t fifo;
    initFifoMed(&fifo);
    
    for (size_t i = 0; i < 10; i++)
    {
        putFifoMed(&fifo, 21 + i);
    }
    
    for (size_t i = 0; i < 10; i++)
    {
        popFifoMed(&fifo);
    }

    popFifoMed(&fifo);
    popFifoMed(&fifo);

    for (size_t i = 0; i < 5; i++)
    {
        putFifoMed(&fifo, 21 + i);
    }

    for (size_t i = 0; i < 6; i++)
    {
        popFifoMed(&fifo);
    }

    printf("[  FifoMed  ]   head_idx: %u, tail_idx: %u, capacity: %u, size: %u\n", fifo.head_idx, fifo.tail_idx, fifo.capacity, fifo.size);
    
    for (size_t i = 0; i < 5; i++)
    {
        putFifoMed(&fifo, 21 + i);
    }

    for (size_t i = 0; i < 6; i++)
    {
        popFifoMed(&fifo);
    }

    printf("[  FifoMed  ]   head_idx: %u, tail_idx: %u, capacity: %u, size: %u\n", fifo.head_idx, fifo.tail_idx, fifo.capacity, fifo.size);

}

void test_LifoSmall()
{
    Lifo_small_t lifo;
    initLifoSmall(&lifo);

    for (size_t i = 0; i < 35; i++)
    {
        putLifoSmall(&lifo, 21 + i);
    }

    for (size_t i = 0; i < 46; i++)
    {
        popLifoSmall(&lifo);
    }

    
    printf("[ LifoSmall ]   head_idx: %u, capacity: %u, size: %u\n", lifo.head_idx, lifo.capacity, lifo.size);
}
