#ifndef PLAY_H
#define PLAY_H

#include <sys/types.h>

// for thread use only.
// take a pointer to a fifo in argument
// Returns nothing
void *playerHandler(void *args);

// Choose the right sound server and initialize it
int init_sound_server();

// Exit the sound server
void exit_sound_server();

// Play raw file at path
int play_file(char *path);

// Play raw stream from a file descriptor
int play_stream(int fd);

#endif
