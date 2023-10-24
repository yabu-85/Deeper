#include "AudioManager.h"
#include "Engine/Audio.h"
#include <cassert>

namespace AudioManager
{
	int hSound_[2];

	void Initialize()
	{
		hSound_[0] = Audio::Load("Sound/JumpingEnd.wav", false, 2);
		assert(hSound_[0] >= 0);

		hSound_[1] = Audio::Load("Sound/maou_game_battle27.wav", false, 2);
		assert(hSound_[1] >= 0);
	}

	void PlaySoundA()
	{
		Audio::Play(hSound_[0], 1.0f);
		Audio::Play(hSound_[1], 1.0f);
	}

}
