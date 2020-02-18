# music-server
This is currently under developement, feel free to contribute.

This music server is currently just a music client.

## Convert your audio files
You must convert your audio file to raw PCM 16bits at 44100 Hz before playing them.
You can use ffmpeg to convert them like so:
```
# For little endian
ffmpeg -i <your_audio_file> -ar 44100 -f s16le -acodec pcm_s16le music.raw

# For big endian
ffmpeg -i <your_audio_file> -ar 44100 -f s16be -acodec pcm_s16be music.raw
```

## Make the projet
Check the dependencies first, you need libpulse in order to compile it at home.
Type make in the project's directory:
```
make
```

## Usage :
```
cat music1.raw music2.raw | bin/music-server
# Or
bin/music-server music1.raw music2.raw
```


