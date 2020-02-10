#include "player.h"
#include "pa.h"
#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int init_sound_server() {
	return init_audio();
}

void exit_sound_server() {
	exit_audio();
}

int play_file(char *path) {
	int fd;
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
	if(fstat(fd, &st) == -1) {
		return 1;
	}
	buffer = malloc(BUFFER_SIZE);
	if(!buffer) {
		fprintf(stderr, "Could not allocate memory\n");
		return 1;
	}
	while((rd = read(fd, buffer, BUFFER_SIZE)) > 0) {
		if(play(buffer, rd) != 0) {
			free(buffer);
			fprintf(stderr, "An error occured while reading the file descriptor\n");
			return 1;
		}
	}
	if(rd < 0) {
		free(buffer);
		fprintf(stderr, "Could not read from the file descriptor\n");
		return 1;
	}
	free(buffer);
	return 0;
}
