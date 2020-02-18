#include "player.h"
#include "config.h"
#include "fifo.h"

// Use pulseaudio interface by default, more supported interface can be added later
// Can be changed in config.h
#ifdef USE_PULSEAUDIO
	#include "pa.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

// Quit global variable, defined in main.c
// Set to 1 when main want to exit
extern int quit;

// Errno, set error variable
extern int errno;

void *playerHandler(void *args) {
	// This function is executed by a player thread.

	// Get the fifo file descriptor
	fifo_t *fifo = (fifo_t *)args;

	// Number of read bytes
	int r;

	// Total read bytes
	int total_read;

	// Buffer, store the raw audio
	void *buffer = malloc(AUDIO_BUFFER_SIZE);

	// Initialize sound server, defined in pa.h
	if(init_sound_server() != 0) {
		fprintf(stderr, "Could not connect to the sound server.\n");	
		exit(EXIT_FAILURE);
	}

	// Playing loop
	while(!quit) {
		// While it's not asked to quit
		total_read = 0;
		while(total_read != AUDIO_BUFFER_SIZE) {
			// Read until the buffer is filled
			r = fifo_read(fifo, buffer + total_read, AUDIO_BUFFER_SIZE - total_read);
			if(r == 0) {
				// There's nothing left to read
				break;
			}
			total_read += r;
		}
		play(buffer, total_read);
	}
	// Exit sound server, defined in pa.h
	exit_sound_server();
	return NULL;
}

int init_sound_server() {
	return init_audio();
}

void exit_sound_server() {
	exit_audio();
}

int play_file(char *path) {
	int fd;
	// Open file for reading only
	fd = open(path, O_RDONLY);
	if(fd == -1) {
		fprintf(stderr, "Could not open the file descriptor of file %s.\n", path);
		return 1;
	}
	return play_stream(fd);
}

int play_stream(int fd) {
	int rd;
	struct stat st;
	void *buffer;
	// Check the file descriptor
	if(fstat(fd, &st) == -1) {
		return 1;
	}
	// allocate memory for the buffer
	buffer = malloc(BUFFER_SIZE);
	if(!buffer) {
		fprintf(stderr, "Could not allocate memory\n");
		return 1;
	}
	while((rd = read(fd, buffer, BUFFER_SIZE)) > 0) {
		// rd bytes have been read from the fd
		if(play(buffer, rd) != 0) {
			// Error while playing the file
			free(buffer);
			fprintf(stderr, "An error occured while reading the file descriptor %d.\n", fd);
			fprintf(stderr, "Errno : %s\n", strerror(errno));
			return 1;
		}
	}
	if(rd < 0) {
		// An error occured
		free(buffer);
		fprintf(stderr, "Could not read from the file descriptor\n");
		return 1;
	}
	free(buffer);
	return 0;
}
