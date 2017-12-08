#pragma once
#include "Audio.h"

class AudioController
{
public:
	static void setMusicVolume(unsigned int volume);
	static unsigned int getMusicVolume();
	static void setSoundVolume(unsigned int volume);
	static unsigned int getSoundVolume();
	static void pauseAllMusic();
	
	/* GAME AUDIO */
	static Audio combat1Audio;
	static Audio introAudio;
	static Audio charselectAudio;
	static Audio navigationAudio;
	static Audio cancelAudio;
	static Audio confirmAudio;

private:
	static unsigned int musicVolume;
	static unsigned int soundVolume;
};