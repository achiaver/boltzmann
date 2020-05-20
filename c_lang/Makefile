.PHONY: all default rasp clean

all: default


default:
	gcc -g -lm -Wall parameters.c matrix.c boltzmann.c -o boltzmann


rasp:
	clang -g -lm -Wall parameters.c matrix.c boltzmann.c -o boltzmann


clean:
	rm -rf boltzmann *.o
