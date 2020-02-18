#ifndef FIFO_H
#define FIFO_H

#include "config.h"
#include <stdint.h>
#include <pthread.h>

typedef struct fifo_t {
	// The buffer
	uint8_t *data;
	// Reading and Writing index
	int current_read;
	int current_write;
	// Size of the buffer
	int size;
	// Flag when stop writing
	// Avoid infinite loop for the reader
	uint8_t eof;
	// Internal carry, tells when write reached the end of the buffer and read did not yet
	uint8_t carry;
	// Condition variable
	// We don't want an infinite loop
	pthread_cond_t update;
	// Mutex, we don't want to read/write the same variable on different threads.
	pthread_mutex_t mux;
} fifo_t;

// Call this function to initialize the fifo
// This is necessary
int fifo_init(fifo_t *fifo);
// Call this function to free the fifo
// Highly recommanded, free some memory
void fifo_free(fifo_t *fifo);

// Write to buffer, does not exceed size, return the read size
int fifo_read(fifo_t *fifo, void *buffer, int size);

// Return the number of written bytes from the buffer, not exceeding size
int fifo_write(fifo_t *fifo, const void *buffer, const int size);

// Return what's available to read
int fifo_available(fifo_t *fifo);

// Inform the FIFO we'll stop writing
void fifo_close(fifo_t *fifo);
#endif
