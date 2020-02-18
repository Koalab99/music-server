#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <pulse/simple.h>
#include <pulse/error.h>

// Pulse audio device
pa_simple *device = NULL;
// Pulse audio sample options
pa_sample_spec sample_spec;


int init_audio() {
	int error = 0;
	// Using 16bits PCM audio using system endianness
	sample_spec.format = PA_SAMPLE_S16LE;

	// Using rate of 44100 Hz
	sample_spec.rate = SAMPLE_RATE;

	// Using 2 channels for stereo output
	sample_spec.channels = 2;

	// Open the default device
	device = pa_simple_new( NULL,			// Use the default server.
				"music-server",		// Our application's name.
				PA_STREAM_PLAYBACK,	// Stream direction
				NULL,			// Use the default device.
				"Music",		// Name of the stream.
				&sample_spec,		// The sample format.
				NULL,			// Use default channel map
				NULL,			// Use default buffering attributes.
				&error			
				);
	if(error > 0) {
		fprintf(stderr, "init_audio: %s\n", pa_strerror(error));
	}
	if(!device) {
		fprintf(stderr, "Cannot find a device\n");
	}
	return error;
}

void exit_audio() {
	pa_simple_free(device);
}

int play(void *data, size_t size) {
	int error;
	if(size == 0 || !data) {
		return 0;
	}
	// Play sound
	if(pa_simple_write(device, data, (size_t)size, &error) != 0) {
		fprintf(stderr, "%s\n", pa_strerror(error));
	}
	return error;
}
