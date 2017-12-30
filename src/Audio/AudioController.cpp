#include "AudioController.h"

Audio AudioController::introAudio(std::string("../../../res/music/intro.wav"), AudioType::MUSIC, AUDIO_DEFAULT_VOLUME);
Audio AudioController::charselectAudio(std::string("../../../res/music/char-select.wav"), AudioType::MUSIC, AUDIO_DEFAULT_VOLUME);
Audio AudioController::combat1Audio(std::string("../../../res/music/BIODRIVE-Neon-City.wav"), AudioType::MUSIC, AUDIO_DEFAULT_VOLUME);
Audio AudioController::navigationAudio(std::string("../../../res/music/navigation2.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::cancelAudio(std::string("../../../res/music/cancel.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::confirmAudio(std::string("../../../res/music/confirm.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);


unsigned int AudioController::musicVolume = AUDIO_DEFAULT_VOLUME;
unsigned int AudioController::soundVolume = AUDIO_DEFAULT_VOLUME;

void AudioController::setMusicVolume(unsigned int volume)
{
	AudioController::introAudio.setVolume(volume);
	AudioController::charselectAudio.setVolume(volume);
	AudioController::combat1Audio.setVolume(volume);
	AudioController::musicVolume = volume;
}

void AudioController::pauseAllMusic() {
	AudioController::introAudio.pause();
	AudioController::charselectAudio.pause();
	AudioController::combat1Audio.pause();
}

unsigned int AudioController::getMusicVolume()
{
	return AudioController::musicVolume;
}

void AudioController::setSoundVolume(unsigned int volume)
{
	// REMEMBER TO REFRESH WHEN ADDING NEW SOUNDS !
	//AudioController::themeAudio.setVolume(volume);
	AudioController::navigationAudio.setVolume(volume);
	AudioController::cancelAudio.setVolume(volume);
	AudioController::confirmAudio.setVolume(volume);
}

unsigned int AudioController::getSoundVolume()
{
	return AudioController::soundVolume;
}