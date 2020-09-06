#include "AudioPlayer.h"
#include "../Game/gameHelper.h"

using namespace irrklang;

AudioPlayer::AudioPlayer()
{
	mAudioPlayer = createIrrKlangDevice();
	mAudioNames.insert(std::pair<AudioName, const char*>(BEGINNING, "./assets/Audio/pacman_beginning.wav"));
	mAudioNames.insert(std::pair<AudioName, const char*>(BACKGROUND, "./assets/Audio/background.mp3"));
	mAudioNames.insert(std::pair<AudioName, const char*>(CHOMP, "./assets/Audio/pacman_chomp.wav"));
	mAudioNames.insert(std::pair<AudioName, const char*>(DEATH, "./assets/Audio/pacman_death.wav"));
	mAudioNames.insert(std::pair<AudioName, const char*>(EAT_FRUIT, "./assets/Audio/pacman_eatfruit.wav"));
	mAudioNames.insert(std::pair<AudioName, const char*>(EAT_GHOST, "./assets/Audio/pacman_eatghost.wav"));
	mAudioNames.insert(std::pair<AudioName, const char*>(EXTRAPAC, "./assets/Audio/pacman_extrapac.wav"));
	mAudioNames.insert(std::pair<AudioName, const char*>(IONTERMISSION, "./assets/Audio/pacman_intermission.wav"));
	mAudioPlayer->addSoundSourceFromFile(mAudioNames[BEGINNING], ESM_AUTO_DETECT, true);
	mAudioPlayer->addSoundSourceFromFile(mAudioNames[CHOMP], ESM_AUTO_DETECT, true);
	mAudioPlayer->addSoundSourceFromFile(mAudioNames[DEATH], ESM_AUTO_DETECT, true);
	mAudioPlayer->addSoundSourceFromFile(mAudioNames[EAT_FRUIT], ESM_AUTO_DETECT, true);
	mAudioPlayer->addSoundSourceFromFile(mAudioNames[EAT_GHOST], ESM_AUTO_DETECT, true);
	mAudioPlayer->addSoundSourceFromFile(mAudioNames[EXTRAPAC], ESM_AUTO_DETECT, true);
	mAudioPlayer->addSoundSourceFromFile(mAudioNames[IONTERMISSION], ESM_AUTO_DETECT, true);
	
}

void AudioPlayer::Drop()
{
	mAudioPlayer->drop();
}


void AudioPlayer::Play(AudioName name, bool looped)
{
	if(!mAudioPlayer->isCurrentlyPlaying(mAudioNames[name]))
	{
		mAudioPlayer->play2D(mAudioNames[name],looped);
	}
}

