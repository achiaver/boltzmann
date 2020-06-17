.PHONY: all clean

OBJS = parameters.o matrix.o boltzmann.o dataset.o

CC = gcc
# CFLAGS = -g -Wall -Wpedantic --std=c2x 		# drand is deprecated
CFLAGS = -g -Wall -Wpedantic 
LIBS = -lm

all: main james

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

main: main.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

james: james.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
 

clean:
	rm -rf main main.o james james.o $(OBJS) *.dsY
