#include "AudioManager.h"
#include "Engine/Audio.h"
#include "GameManager/GameManager.h"
#include "Enemy/EnemyManager.h"
#include <cassert>
#include <vector>

namespace AudioManager
{
	std::vector<int> hSound_;
	struct AudioData {
		std::string name;
		bool isLoop;
		unsigned short max;
	};
	std::vector<AudioData> sceneTable;

    float gameVolue_ = 1.0f;

}

void AudioManager::Initialize()
{
	sceneTable = {
		{ "JumpingEnd", false, 5 },
		{ "maou_game_battle27", false, 2 }
	};

}

void AudioManager::Release()
{
	Audio::Release();
}

void AudioManager::SetSceneData(AUDIO_SCENE scene)
{
    switch (scene) {
    case PLAY:
        sceneTable = {
            {"Sound/RobotHit.wav", false, 3},

        };
        break;
    case OTHER:
        sceneTable = {
            {"Sound/RobotHit.wav", false, 3},
        };
        break;
    }

    //hSound_ベクターのサイズを設定
    hSound_.resize(sceneTable.size());
    for (int i = 0; i < sceneTable.size(); i++) {
        hSound_[i] = Audio::Load(sceneTable[i].name, sceneTable[i].isLoop, sceneTable[i].max);
        assert(hSound_[i] >= 0);
    }
}

void AudioManager::Play()
{
	Audio::Play(hSound_[0], 1.0f);
	Audio::Play(hSound_[1], 1.0f);
}

void AudioManager::Play(XMFLOAT3 position, float range)
{
	Audio::Play(hSound_[0], 1.0f);
	GameManager::GetEnemyManager()->PlayAtPosition(position, range);

}

/*
void PlaySoundMa(TITLE_AUDIO i, float volume)
{
    Audio::Play(hSound_[i], volume * gameVolue_);
}

void StopSoundMa(PLAY_AUDIO i)
{
    Audio::Stop(hSound_[i]);
}
*/
