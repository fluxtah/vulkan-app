#ifndef APP_SOUND_H
#define APP_SOUND_H

#include <stdio.h>
#include <stdlib.h>
#include <OpenAL/al.h>
#include <OpenAL//alc.h>
#include "libs/dr_wav.h"

typedef struct AudioSource {
    short *data;
    size_t bufferSize;
    uint32_t sampleRate;
    ALuint buffer;
    ALuint source;
} AudioSource;

typedef struct AudioContext {
    ALCdevice *device;
    ALCcontext *context;
} AudioContext;

AudioContext *createAudioContext();

void destroyAudioContext(AudioContext *context);

AudioSource *loadSound(const char *filename);

void playSound(AudioSource *audioSource);

void destroySound(AudioSource *sound);

int hasSoundFinishedPlaying(AudioSource *sound);

#endif //APP_SOUND_H
