#include "fifo.h"
#include "config.h"
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Should initialize properly the fifo
int fifo_init(fifo_t *fifo) {
	fifo->current_read = 0;
	fifo->current_write = 0;
	fifo->size = BUFFER_SIZE;
	fifo->eof = 0;
	fifo->data = malloc(fifo->size);
	fifo->carry = 0;
	pthread_mutex_init(&fifo->mux, NULL);
	pthread_cond_init(&fifo->update, NULL);

	// Success if data has been allocated
	return fifo->data == NULL;
}

void fifo_free(fifo_t *fifo) {
	if(fifo->data) {
		free(fifo->data);
	}
}

// Read up to size bytes from the fifo, returns the number of read bytes
int fifo_read(fifo_t *fifo, void *buffer, int size) {
	int max = 0;
	// Lock the fifo's mutex
	pthread_mutex_lock(&fifo->mux);
	// Avoid active waiting
	while(fifo_available(fifo) == 0 && fifo->eof == 0 ) {
		pthread_cond_wait(&fifo->update, &fifo->mux);
	}
	if(fifo->carry == 1) {
		// Write has one more cycle in the buffer
		// We can read until the end of the buffer

		// Check the max we can copy:
		max = fifo->size - fifo->current_read;
		if(max > size) {
			max = size;
		}
		// Copy from the fifo to the buffer
		memcpy(buffer, fifo->data + fifo->current_read, max);
		// Update the fifo read index
		fifo->current_read = (fifo->current_read + max) % fifo->size;
		// Reset the carry if reaching the end of the buffer
		if(fifo->current_read == 0 && max != 0) {
			fifo->carry = 0;
		}
	}
	else if (fifo->current_read < fifo->current_write) {
		// Write is in the same cycle as read
		// We can read until the write index

		// Check the max we can copy
		max = fifo->current_write - fifo->current_read;
		if(max > size) {
			max = size;
		}
		// Copy from the fifo to the buffer
		memcpy(buffer, fifo->data + fifo->current_read, max);
		// Update the fifo read index
		fifo->current_read = (fifo->current_read + max);
	}
	// unlock the condition and the mutex for other threads
	pthread_cond_broadcast(&fifo->update);
	pthread_mutex_unlock(&fifo->mux);
	return max;
}

// Will write exactly size bytes from the buffer to the fifo
int fifo_write(fifo_t *fifo, const void *buffer, const int size) {
	int max;
	// Write counter
	int written = 0;
	// We write the entire message to the fifo
	while(written != size) { 
		// Lock the mutex
		pthread_mutex_lock(&fifo->mux);
		max = 0;
		while(fifo->current_read == fifo->current_write && fifo->carry == 1) { 
			// We wait for the fifo to be available
			pthread_cond_wait(&fifo->update, &fifo->mux);
		}
		if(fifo->carry == 1) {
			// Write has one more cycle in the buffer
			// Write until the read index

			// Check the max we can copy
			max = fifo->current_read - fifo->current_write;
			if(max > size - written) {
				max = size - written;
			}
			// Copy from the buffer to the fifo
			memcpy(fifo->data + fifo->current_write, buffer + written, max);
			// Update the fifo's write index
			fifo->current_write = (fifo->current_write + max);
		}
		else {
			// Write is in the same cycle as read
			// Write until the end of the buffer

			// Check the max we can copy
			max = fifo->size - fifo->current_write;
			if(max > size - written) {
				max = size - written;
			}
			// Copy from the buffer to the fifo
			memcpy(fifo->data + fifo->current_write, buffer + written, max);
			// Update the write index
			fifo->current_write = (fifo->current_write + max);
			fifo->current_write %= fifo->size;
			// Set the carry in case we reached the end of the buffer
			if(fifo->current_write == 0 && max != 0) {
				fifo->carry = 1;
			}
		}
		// Unlock the condition and the mutex for other threads
		pthread_cond_broadcast(&fifo->update);
		pthread_mutex_unlock(&fifo->mux);
		// Keep trace of how much we wrote
		written += max;
	}
	return max;
}

int fifo_available(fifo_t *fifo) {
	int size = 0;
	if(fifo->carry == 1) {
		// Write index < Read index
		// Carry is set to 1
		size = fifo->size - fifo->current_read;
		size += fifo->current_write;
	}
	else { 
		// Write index > Read index
		// Carry is set to 0
		size = fifo->current_write - fifo->current_read - 1;
	}
	return size;
}

void fifo_close(fifo_t *fifo) {
	// Set end of file
	fifo->eof = 1;
	// Unlock conditions for reading thread
	pthread_cond_broadcast(&fifo->update);
}
