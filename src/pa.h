#ifndef PULSE_AUDIO_H
#define PULSE_AUDIO_H

#include <sys/types.h>

int init_audio();

void exit_audio();

int play(void *data, size_t bytes);

#endif
