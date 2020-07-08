.PHONY: all clean

OBJS = parameters.o matrix.o boltzmann.o dataset.o random_gen.o

# drand is deprecated
# CFLAGS = -g -Wall -Wpedantic --std=c2x

# DEBUG
CFLAGS = -g -Wall -Wpedantic

# OPTIMIZED
CFLAGS = -O3 -Wall -Wpedantic
LIBS = -lm -lgsl -lgslcblas
CC = gcc

all: main james mnist_02

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

main: main.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

james: james.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
 
mnist_02: mnist_02.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf main main.o james james.o mnist_02 mnist_02.o $(OBJS) *.dsY
