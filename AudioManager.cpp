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

	void Initialize()
	{
		sceneTable = {
			{ "JumpingEnd", false, 5 },
			{ "maou_game_battle27", false, 2 }
		};

		hSound_.resize(sceneTable.size()); //hSound_ベクターのサイズを設定
		for (int i = 0; i < sceneTable.size(); i++) {
			hSound_[i] = Audio::Load("Sound/" + sceneTable[i].name + ".wav", sceneTable[i].isLoop, sceneTable[i].max);
			assert(hSound_[i] >= 0);
		}
	}

	void Release()
	{
		Audio::Release();
	}

	void Play()
	{
		Audio::Play(hSound_[0], 1.0f);
		Audio::Play(hSound_[1], 1.0f);
	}

	void Play(XMFLOAT3 position, float range)
	{
		Audio::Play(hSound_[0], 1.0f);
		GameManager::GetEnemyManager()->PlayAtPosition(position, range);

	}

}
