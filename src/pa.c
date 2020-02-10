#include <stdio.h>
#include <stdlib.h>
#include <pulse/simple.h>
#include <pulse/error.h>

static pa_simple *device = NULL;
static pa_sample_spec sample_spec;

int init_audio() {
	// Setting stereo output of pulseaudio
	sample_spec.format = PA_SAMPLE_S16NE;
	sample_spec.rate = 44100;
	sample_spec.channels = 2;

	device = pa_simple_new( NULL,			// Use the default server.
				"music-server",		// Our application's name.
				PA_STREAM_PLAYBACK,	// Stream direction
				NULL,			// Use the default device.
				"Music",		// Name of the stream.
				&sample_spec,		// The sample format.
				NULL,			// Use default channel map
				NULL,			// Use default buffering attributes.
				NULL			// Ignore error code.
				);
	if(!device) {
		fprintf(stderr, "Cannot find a device\n");
		return 1;
	}
	return 0;
}

void exit_audio() {
	pa_simple_free(device);
}

int play(void *data, size_t size) {
	int error;
	pa_simple_write(device, data, size, &error);
	return error;
}
