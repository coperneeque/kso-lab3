# Makefile
CFLAGS = -c
LDFLAGS = -lrt -lpthread

all: test_flags main.o fifo_big.o fifo_med.o lifo_small.o simple_test.o shared_mem.o consumer.o producer.o textcolour.o
	gcc $(LDFLAGS) main.o fifo_big.o fifo_med.o lifo_small.o simple_test.o shared_mem.o textcolour.o -o main
	gcc $(LDFLAGS) consumer.o fifo_big.o fifo_med.o lifo_small.o shared_mem.o textcolour.o -o consumer
	gcc $(LDFLAGS) producer.o fifo_big.o fifo_med.o lifo_small.o shared_mem.o textcolour.o -o producer

main.o: main.c fifo_big.h fifo_med.h lifo_small.h simple_test.h test_flags.h textcolour.h
	gcc $(CFLAGS) main.c

fifo_big.o: fifo_big.h fifo_big.c simple_test.h test_flags.h textcolour.o
	gcc $(CFLAGS) fifo_big.c

fifo_med.o: fifo_med.h fifo_med.c simple_test.h test_flags.h
	gcc $(CFLAGS) fifo_med.c

lifo_small.o: lifo_small.h lifo_small.c simple_test.h test_flags.h
	gcc $(CFLAGS) lifo_small.c

shared_mem.o: shared_mem.h shared_mem.c test_flags.h
	gcc $(CFLAGS) shared_mem.c

consumer.o: consumer.c shared_mem.h fifo_big.h fifo_med.h lifo_small.h textcolour.h
	gcc $(CFLAGS) consumer.c

producer.o: producer.c shared_mem.h fifo_big.h fifo_med.h lifo_small.h textcolour.h
	gcc $(CFLAGS) producer.c

textcolour.o: textcolour.h textcolour.c
	gcc $(CFLAGS) textcolour.c

simple_test.o: simple_test.h simple_test.c fifo_big.h fifo_med.h lifo_small.h
	gcc $(CFLAGS) simple_test.c

test_flags: test_flags.h

clean:
	rm -f *.o main consumer producer

