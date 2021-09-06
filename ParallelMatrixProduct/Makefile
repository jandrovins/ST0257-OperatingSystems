CC            = gcc
PROGRAM_NAME  = matrixMultiplication
EXTRA_FLAGS   = -pthread -O3 -std=c11 -Wall -Wextra
ALL_CFLAGS    = $(CFLAGS) $(EXTRA_FLAGS)


.DEFAULT_GOAL := all
.PHONY: all
all: matrixMultiplication

matrixMultiplication: matrixMultiplication.c
	$(CC) $(ALL_CFLAGS) -o $@ $^
