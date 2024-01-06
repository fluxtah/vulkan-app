#include "include/sound.h"

AudioSource *loadSound(const char *filename) {
    // Load WAV file
    drwav wav;
    if (!drwav_init_file(&wav, filename, NULL)) {
        fprintf(stderr, "Error opening WAV file\n");
        return NULL;
    }

    if (wav.bitsPerSample != 16) {
        fprintf(stderr, "Unsupported bit depth: %d\n", wav.bitsPerSample);
        drwav_uninit(&wav);
        return NULL;
    }

    size_t bufferSize = wav.totalPCMFrameCount * wav.channels * sizeof(short);
    short *pSampleData = (short *) malloc(bufferSize);
    if (pSampleData == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        drwav_uninit(&wav);
        return NULL;
    }

    drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pSampleData);

    drwav_uninit(&wav);

    AudioSource *sound = malloc(sizeof(AudioSource));
    sound->data = pSampleData;
    sound->bufferSize = bufferSize;
    sound->sampleRate = wav.sampleRate;

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_STEREO16, sound->data, sound->bufferSize, sound->sampleRate);
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    //alSourcef(source, AL_PITCH, 1.5f);

    sound->buffer = buffer;
    sound->source = source;

    return sound;
}

void destroySound(AudioSource *sound) {
    alDeleteSources(1, &sound->source);
    alDeleteBuffers(1, &sound->buffer);
    free(sound->data);
    free(sound);
}

int hasSoundFinishedPlaying(AudioSource *sound) {
    ALint sourceState;
    alGetSourcei(sound->source, AL_SOURCE_STATE, &sourceState);
    if (sourceState == AL_PLAYING) {
        return 1;
    }

    return -1;
}

void playSound(AudioSource *audioSource) {
    alSourcePlay(audioSource->source);
}

AudioContext *createAudioContext() {
    // Initialize OpenAL
    ALCdevice *device = alcOpenDevice(NULL);
    if (!device) {
        fprintf(stderr, "Error opening OpenAL device\n");
        return NULL;
    }

    ALCcontext *context = alcCreateContext(device, NULL);
    if (!context || !alcMakeContextCurrent(context)) {
        fprintf(stderr, "Error creating OpenAL context\n");
        alcCloseDevice(device);
        return NULL;
    }

    AudioContext *audioContext = malloc(sizeof(AudioContext));
    audioContext->device = device;
    audioContext->context = context;
    return audioContext;
}

void destroyAudioContext(AudioContext *context) {
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context->context);
    alcCloseDevice(context->device);
}

