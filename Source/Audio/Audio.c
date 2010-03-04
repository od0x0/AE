 #include <SDL_mixer/SDL_mixer.h>
 
void AEAudioQuit(void){
	Mix_CloseAudio();
	while(Mix_Init(0)) Mix_Quit(); 
}

void AEAudioInit(char numberOfChannels){
	//Simply taken from the docs
	// load support for the OGG and MP3 sample/music formats 
	int flags=MIX_INIT_OGG|MIX_INIT_MP3; 
	int initted=Mix_Init(flags); 
	if(initted&flags != flags) { 
		printf("Mix_Init: Failed to init required ogg and mp3 support!\n"); 
		printf("Mix_Init: %s\n", Mix_GetError()); 
		exit(1);
		// handle error 
	}  
	// open 44.1KHz, signed 16bit, system byte order, 
	// stereo audio, using 1024 byte chunks 
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) { 
		printf("Mix_OpenAudio: %s\n", Mix_GetError()); 
		exit(1); 
	} 
	Mix_AllocateChannels(numberOfChannels); 
	//atexit(AEAudioQuit);
}


int AEModeIsMusic=0;//Cleans up an extra parameter

void* AEAudioLoad(const char* filename){
	void* sound=NULL;
	
	if(AEModeIsMusic)
		sound=(void*)Mix_LoadMUS(filename); 
	else
		sound=Mix_LoadWAV(filename); //They call it LoadWAV, but apparently, it can load wav, aiff, riff, ogg, and voc as well.... Weird naming.
	
	if(!sound) { 
		printf("AEAudioLoad(\"%s\") (mode:%s): %s\n",filename, (AEModeIsMusic?"Music":"Sample"),Mix_GetError()); 
		exit(1);
	} 
	return sound;
}
int AEAudioPlay(void* sound,int loop){
	int channel=-1;
	if(!AEModeIsMusic) channel=Mix_PlayChannel(-1, (Mix_Chunk*)sound, loop);
	else channel=Mix_PlayMusic((Mix_Music*)sound, loop);
	if(channel==-1) printf("AEAudioPlay(%p,%i) (mode:%s): %s\n",sound,loop,(AEModeIsMusic?"Music":"Sample"),Mix_GetError()); 
	return channel;
}

int AEAudioPause(int sound){
	if(AEModeIsMusic) Mix_PauseMusic();
	else Mix_Pause(sound);
	return sound;
}

int AEAudioResume(int sound){
	if(AEModeIsMusic) Mix_ResumeMusic();
	else Mix_Resume(sound);
	return sound;
}

int AEAudioVolume(int sound,float level){
	if(AEModeIsMusic) Mix_VolumeMusic(MIX_MAX_VOLUME*level);
	else Mix_Volume(sound,MIX_MAX_VOLUME*level);
	return sound;
}

void AEAudioDelete(void* sound){
	if(AEModeIsMusic) Mix_FreeMusic((Mix_Music*)sound);
	else Mix_FreeChunk((Mix_Chunk*)sound);
}