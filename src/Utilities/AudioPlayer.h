#include <irrKlang/irrKlang.h>
#include <string>
#include <map>


//Signleton class used across the game. 
class AudioPlayer
{

public:
	enum AudioName
	{
		BEGINNING,
		BACKGROUND,
		CHOMP,
		DEATH,
		EAT_FRUIT,
		EAT_GHOST,
		EXTRAPAC,
		IONTERMISSION
	};

	AudioPlayer(const AudioPlayer&) = delete;
	AudioPlayer& operator=(const AudioPlayer&) = delete;
	AudioPlayer(AudioPlayer&&) = delete;
	AudioPlayer& operator=(AudioPlayer&&) = delete;
	static auto& instance() {
		static AudioPlayer player;
		return player;
	}

	void Drop();
	void Play(AudioName name, bool lopped = false);

private:
	AudioPlayer();
	irrklang::ISoundEngine* mAudioPlayer;
	std::map<AudioName, const char*> mAudioNames;

};