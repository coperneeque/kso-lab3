# Makefile
CFLAGS = -c
LDFLAGS = -lrt -lpthread

all: test_flags main.o fifo_big.o fifo_med.o lifo_small.o simple_test.o shared_mem.o consumer.o producer.o
	gcc $(LDFLAGS) main.o fifo_big.o fifo_med.o lifo_small.o simple_test.o shared_mem.o -o main
	gcc $(LDFLAGS) consumer.o fifo_big.o fifo_med.o lifo_small.o shared_mem.o -o consumer
	gcc $(LDFLAGS) producer.o fifo_big.o fifo_med.o lifo_small.o shared_mem.o -o producer

main.o: main.c test_flags.h simple_test.h
	gcc $(CFLAGS) main.c -o main.o

fifo_big.o: fifo_big.h fifo_big.c simple_test.h test_flags.h
	gcc $(CFLAGS) fifo_big.c -o fifo_big.o

fifo_med.o: fifo_med.h fifo_med.c simple_test.h test_flags.h
	gcc $(CFLAGS) fifo_med.c -o fifo_med.o

lifo_small.o: lifo_small.h lifo_small.c simple_test.h test_flags.h
	gcc $(CFLAGS) lifo_small.c -o lifo_small.o

shared_mem.o: shared_mem.h shared_mem.c
	gcc $(CFLAGS) shared_mem.c -o shared_mem.o

consumer.o: consumer.h consumer.c 
	gcc $(CFLAGS) consumer.c -o consumer.o

producer.o: producer.c
	gcc $(CFLAGS) producer.c -o producer.o

simple_test.o: simple_test.h simple_test.c fifo_big.h fifo_med.h lifo_small.h
	gcc $(CFLAGS) simple_test.c -o simple_test.o

test_flags: test_flags.h

clean:
	rm -f *.o main consumer producer

