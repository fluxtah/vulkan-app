#ifndef APP_SOUND_H
#define APP_SOUND_H

#include <stdio.h>
#include <stdlib.h>
#include <OpenAL/al.h>
#include <OpenAL//alc.h>
#include "libs/dr_wav.h"
#include "model.h"

typedef struct Sound {
    short *data;
    size_t bufferSize;
    uint32_t sampleRate;
    ALuint buffer;
    ALuint source;
} Sound;

typedef struct AudioContext {
    ALCdevice *device;
    ALCcontext *context;
} AudioContext;

AudioContext *createAudioContext();

void destroyAudioContext(AudioContext *context);

Sound *loadSound(const char *filename, CreateSoundInfo *info);
void destroySound(Sound *sound);

void playSound(Sound *sound);
int isSoundPlaying(Sound *sound);
void stopSound(Sound *sound);
void setSoundPitch(Sound *sound, float pitch);

#endif //APP_SOUND_H
