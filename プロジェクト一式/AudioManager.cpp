#include "AudioManager.h"
#include "Engine/Audio.h"
#include "Engine/Global.h"
#include "GameManager/GameManager.h"
#include "Player/Player.h"
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

    float gameVolue_;

}

void AudioManager::Initialize()
{
    SetAudioData();
    gameVolue_ = 0.2f;

}

void AudioManager::Release()
{
	Audio::Release();
}

void AudioManager::SetAudioData()
{
    //enumの順番に
    sceneTable = {
        {"Sound/ButtonWithin.wav", false, 5},
        {"Sound/ButtonPush.wav", false, 3},
        {"Sound/PauseOpen.wav", false, 2},
        {"Sound/PauseClose.wav", false, 2},
        {"Sound/SwordWield1.wav", false, 3},
        {"Sound/SwordWield2.wav", false, 3},
        {"Sound/SwordWield3.wav", false, 3},
        {"Sound/BulletHit.wav", false, 3},
    };

    //hSound_ベクターのサイズを設定
    hSound_.resize(sceneTable.size());
    for (int i = 0; i < sceneTable.size(); i++) {
        hSound_[i] = Audio::Load(sceneTable[i].name, sceneTable[i].isLoop, sceneTable[i].max);
        assert(hSound_[i] >= 0);
    }
}

void AudioManager::Play(AUDIO_ID id, float volume)
{
	Audio::Play(hSound_[(int)id], volume * gameVolue_);

}

void AudioManager::Play(AUDIO_ID id, XMFLOAT3 position, float range, float volume)
{
    //距離によって音量を変える
    float dist = DistanceCalculation(GameManager::GetPlayer()->GetPosition(), position);
    if(dist > range) volume = volume * (dist / range);

	Audio::Play(hSound_[(int)id], volume * gameVolue_);
	GameManager::GetEnemyManager()->PlayAtPosition(position, range);

}