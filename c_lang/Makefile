.PHONY: all clean

TARGETS = boltzmann

OBJS = parameters.o matrix.o boltzmann.o

CC = gcc
# CFLAGS = -g -Wall -Wpedantic --std=c2x 		# drand is deprecated
CFLAGS = -g -Wall -Wpedantic 
LIBS = -lm


all: $(TARGETS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

$(TARGETS): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
 
clean:
	rm -rf $(TARGETS) $(OBJS) $(TARGETS).dsY
