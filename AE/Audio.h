#pragma once
#include "Core.h"

void AEAudioQuit(void);
void AEAudioInit(char numberOfChannels);
extern int AEAudioMusicMode;
void* AEAudioLoad(const char* filename);
int AEAudioPlay(void* sound,int loop);
int AEAudioPause(int sound);
int AEAudioResume(int sound);
int AEAudioVolume(int sound,float level);
void AEAudioDelete(void* sound);
