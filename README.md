# music-server
This is a simple music player for now.
It uses PulseAudio as the audio server.
It plays only PCM 16bits samples with at 44100Hz.
You can convert your music file with ffmpeg with:
```
make
ffmpeg -i <your_audio_file> -ar 44100 -f s16le -acodec pcm_s16le music.raw

# Then
cat music.raw | bin/music-server
# Or
bin/music-server music.raw
```
