//Todo: intros/outros

#include <SDL_mixer.h>
Mix_Music* musicList[MAX_MUS_COUNT];
Mix_Chunk* soundEffects[MAX_SEF_COUNT];

typedef struct SE{
	int index;
	int channel;
	char volume;
} SE;

int resetAudio(){
	Mix_VolumeMusic(mus_volume);
}

int audioSetup(){
	DEBUG("Setting up audio...");
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0){ERROR(1,"Failed to initialize SDL_mixer");}
	Mix_VolumeMusic(mus_volume);
	return 0;
}

int loadMusic(){
	DEBUG("Loading music...");
	musicList[0]=NULL;
	for(int i=1;i<mus_count;i++){
		DEBUG("Loading %s from %s...",mus_names[i],mus_paths[i]);
		musicList[i]=Mix_LoadMUS(mus_paths[i]);
		if(musicList[i]==NULL){ERROR(1,"Failed to load music from %s.",mus_paths[i]);}
	}
	DEBUG("Done.");
	return 0;
}

int loadSE(){
	DEBUG("Loading sound effects...");
	soundEffects[0]=NULL;
	for(int i=1;i<sef_count;i++){
		//DEBUG("Loading %s from %s...",sef_names[i],sef_paths[i]);
		soundEffects[i]=Mix_LoadWAV(sef_paths[i]);
		if(soundEffects[i]==NULL){ERROR(1,"Failed to load sound from %s.",sef_paths[i]);}
	}
	DEBUG("Done.");
	return 0;
}

int playSE(int i,int vol,int effect){
	if(i>0){
		DEBUG("Playing sound effect %s.",sef_names[i]);
		Mix_Volume(Mix_PlayChannel(-1,soundEffects[i],0),vol*sef_volume);
	}
	return 0;
}

int silenceMusic(int fade){
	if(Mix_PlayingMusic()){
		DEBUG("Fading out music.");
		if(fade){
			Mix_FadeOutMusic(100*fade);
		}else{
			Mix_HaltMusic();
		}
	}
	return 0;
}

int playMusic(int i,int fade,int loops){
	if(i==0){silenceMusic(fade);}
	if(fade){
		DEBUG("Fading in music %s.",mus_names[i]);
		if(Mix_PlayingMusic()){
			Mix_FadeOutMusic(50*fade);
			Mix_FadeInMusic(musicList[i],-1,50*fade);
		}else{
			Mix_FadeInMusic(musicList[i],-1,100*fade);
		}
	}else{
		DEBUG("Playing music %s %d",mus_names[i],i);
		Mix_PlayMusic(musicList[i],loops-1);
	}
	return 0;
}

int audioCleanup(){
	for(int i=1;i<mus_count;i++){
		Mix_FreeMusic(musicList[i]);
	}
	for(int i=0;i<sef_count;i++){
		Mix_FreeChunk(soundEffects[i]);
	}
	Mix_Quit();
}