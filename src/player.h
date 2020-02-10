#ifndef PLAY_H
#define PLAY_H

#include <sys/types.h>

int init_sound_server();
void exit_sound_server();
int play_file(char *path);
int play_stream(int fd);

#endif
