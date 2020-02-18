#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "player.h"
#include "config.h"
#include "fifo.h"

// Errno
extern int errno;

// Global quit, used to inform every threads we want to quit
int quit = 0;

// Write raw/pcm files in here and it will be played
fifo_t audio_stream;

void title() {
	// Say hello
	printf(	"===========================\n"
		" Welcome to music-server !\n"
		"===========================\n");
}

int main(int argc, char **argv) {
	// The player's own thread
	pthread_t player;
	uint8_t *buffer = malloc(BUFFER_SIZE);
	int r;
	
	// Print the welcoming screen
	title();

	// Initialize the FIFO for audio stream
	if(fifo_init(&audio_stream) != 0) {
		fprintf(stderr, "Could not initialize the FIFO\n");	
		exit(EXIT_FAILURE);
	}

	// A new thread joined the team, he'll output any raw data we send to audio_stream
	pthread_create(&player, NULL, playerHandler, &audio_stream);

	if(argc > 1) {
		// Use the file given by command line argument
		FILE *file;
		for(int i = 1; i < argc; i++) {
			if((file = fopen(argv[i], "rb")) == NULL) {
				fprintf(stderr, "Could not open %s.\n", argv[1]);
				fprintf(stderr, "Errno : %d : %s\n", errno, strerror(errno));
				continue;
			}
			// Read the file and write it to the fifo
			while((r = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, file)) > 0) {
				fifo_write(&audio_stream, buffer, r);
			}
			fclose(file);
		}
	}
	else {
		// Reading from the standard input (aka pipe, etc) and write it to the fifo
		while((r = fread(buffer, 1, BUFFER_SIZE, stdin)) > 0) {
			fifo_write(&audio_stream, buffer, r);
		}
	}
	// Close the fifo, should terminate the read thread
	fifo_close(&audio_stream);

	// Inform all threads we want to leave
	quit = 1;

	// Wait for the threads
	pthread_join(player, NULL);

	// De-allocate fifo memory
	fifo_free(&audio_stream);
	exit(EXIT_SUCCESS);
}
