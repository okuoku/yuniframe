#include <yfrm.h>

#include <string.h>
#include <stdio.h>
#include <SDL.h>

static SDL_AudioDeviceID audiodevice;
static SDL_mutex* audiolock;
static int yfrm_audio_state = 0;

static void
audio_sync_init(void){
    audiolock = SDL_CreateMutex();
    yfrm_audio_state = 1;
}

YFRM_API void
yfrm_audio_pause0(void){
    if(yfrm_audio_state == 0){
        audio_sync_init();
    }
    SDL_LockMutex(audiolock);
    if(yfrm_audio_state != 1){
        SDL_CloseAudioDevice(audiodevice);
        yfrm_audio_state = 1;
    }
    SDL_UnlockMutex(audiolock);
}

static void
queueaudio(float* ch0, float* ch1, int32_t samples){
    int i;
    const size_t bufsiz = sizeof(float) * 2 * samples;
    float* buf;
    buf = malloc(bufsiz);
    /* Interleave data to LR order */
    for(i=0;i!=samples;i++){
        buf[i*2] = ch0[i];
        buf[i*2+1] = ch1[i];
    }
    i = SDL_QueueAudio(audiodevice, buf, bufsiz);
    if(i < 0){
        printf("Failed to queue audio: %s\n", SDL_GetError());
    }
    free(buf);
    //printf("Queued: %d\n", SDL_GetQueuedAudioSize(audiodevice));
}

YFRM_API void
yfrm_audio_enqueue0(float* ch0, float* ch1, int32_t samples){
    SDL_AudioSpec want, have;
    if(yfrm_audio_state == 0){
        audio_sync_init();
    }

    SDL_LockMutex(audiolock);
    if(yfrm_audio_state == 1){
        /* Audio is not started */
        memset(&want, 0, sizeof(want));
        memset(&have, 0, sizeof(have));
        want.freq = 48000;
        want.format = AUDIO_F32;
        want.channels = 2;
        want.samples = 256;
        want.callback = NULL;
        audiodevice = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
        if(! audiodevice){
            printf("Failed to open audio: %s\n", SDL_GetError());
        }else{
            yfrm_audio_state = 2;
        }
    }

    if(yfrm_audio_state == 2){
        /* Paused */
        yfrm_audio_state = 3;
        queueaudio(ch0,ch1,samples);
        SDL_PauseAudioDevice(audiodevice, 0);
    }else if(yfrm_audio_state == 3){
        /* Playing */
        queueaudio(ch0,ch1,samples);
    }
    SDL_UnlockMutex(audiolock);
}

