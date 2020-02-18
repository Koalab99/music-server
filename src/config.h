#ifndef CONFIG_H
#define CONFIG_H

// Configuration file for music-server

// The size of the audio stream buffer
#define AUDIO_BUFFER_SIZE 2048

// Path for the FIFO stack
#define FIFO_PATH "/tmp/music-server/fifo"

// FIFO buffer size
#define BUFFER_SIZE 2048

// Use PulseAudio
#define USE_PULSEAUDIO

// Set the sample rate (in Hz)
#define SAMPLE_RATE 44100

#endif
