.PHONY: all clean

OBJS = parameters.o matrix.o boltzmann.o dataset.o random_func.o pcg_basic.o

# drand is deprecated
# CFLAGS = -g -Wall -Wpedantic --std=c2x

# DEBUG
# CFLAGS = -g -Wall -Wpedantic

# OPTIMIZED
CFLAGS = -O3 -Wall -Wpedantic -g
LIBS = -lm -lgsl -lgslcblas
CC = gcc

all: main james smolensky_make

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

main: main.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

james: james.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
 
smolensky_make: smolensky_make.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf main main.o james james.o smolensky_make smolensky_make.o $(OBJS) *.dsY *.o
