#pragma once
#include "Core.h"
#include "Math3D.h"

void AEAudioQuit(void);
void AEAudioInit(char numberOfChannels);
extern int AEAudioMusicMode;
void* AEAudioLoad(const char* filename);//Only plan on OGG support, everything else is implementation dependent
int AEAudioPlay(void* buffer,int loop);
int AEAudioPause(int channel);
int AEAudioResume(int channel);
int AEAudioVolume(int channel,float level);
void AEAudioDelete(void* buffer);
void AEAudioPosition(int channel,float x,float y,float z);//-1 is listener