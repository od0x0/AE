#include "../AE/Core.h"
#include "../AE/Audio.h"

int main(int argc,char** argv){
	AEInit("Audio Test",800,500);
	AEAudioInit(16);
	AEAudioMusicMode=1;
	void* music=AEAudioLoad("Data/Beat.mp3");
	AEAudioPlay(music,-1);
	AEStart(NULL);
}
