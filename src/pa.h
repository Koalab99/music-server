#ifndef PULSE_AUDIO_H
#define PULSE_AUDIO_H

#include <sys/types.h>

// Initialize pulseaudio
int init_audio();

// Free pulseaudio 
void exit_audio();

// Play bytes bytes of data
int play(void *data, size_t bytes);

#endif
