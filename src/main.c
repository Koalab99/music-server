#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "player.h"

void title() {
	printf(	"===========================\n"
		" Welcome to music-server !\n"
		"===========================\n");
}

int main(int argc, char **argv) {
	title();
	if(init_sound_server() != 0) {
		fprintf(stderr, "Could not connect to the sound server.\n");	
		exit(EXIT_FAILURE);
	}
	if(argc > 1) {
		if(play_file(argv[1]) != 0) {
			fprintf(stderr, "Could not play %s.\n", argv[1]);	
			exit(EXIT_FAILURE);
		}
	}
	else {
		if(play_stream(STDIN_FILENO) != 0) {
			fprintf(stderr, "Could not play song on stdin");
			exit(EXIT_FAILURE);
		}
	}
	exit_sound_server();
	exit(EXIT_SUCCESS);
}
