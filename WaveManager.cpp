#include "WaveManager.h"
#include "GameManager.h"
#include "DifficultyManager.h"
#include "Engine/SceneManager.h"
#include "Enemy/EnemyManager.h"
#include <vector>

namespace WaveManager {
	enum STAGE_INDEX {
		STAGE1 = 0,
		STAGE2,
		STAGE3,
		MAX,
	};

	struct StageData {
		int waveCount;		//Wave�̐�
		int enemyCount;		//�PWave�ŏo�Ă���G�̐�
		std::vector<ENEMY_TYPE> spawnEnemy;
		
		StageData(int wave, std::vector<ENEMY_TYPE> list, int enemy) : waveCount(wave), spawnEnemy(list), enemyCount(enemy) {};
	} 
	data_[MAX] = {
		StageData(0, {}, 0 ),
		StageData(3, { ENEMY_STONEGOLEM, ENEMY_THROW }, 5),
		StageData(1, { ENEMY_STONEGOLEM, ENEMY_THROW }, 3),
	};

	int currentDataIndex = 0;
	int waveCount_ = 0;

	//-----------------------------------------------------------------

	void Update()
	{
		//�����EWave�N���A
		if (waveCount_ >= 1 && GameManager::GetEnemyManager()->IsEnemyListEmpty()) {
			waveCount_--;
			SetWaveData();

		}
	}

	void SetStageData() {
		SCENE_ID cs = GameManager::GetSceneManager()->GetNextSceneID();
		currentDataIndex = 0;

		if (cs == SCENE_ID_STAGE1) currentDataIndex = 0;
		else if (cs == SCENE_ID_STAGE2) currentDataIndex = 1;
		else if (cs == SCENE_ID_STAGE3) currentDataIndex = 2;

		waveCount_ = data_[currentDataIndex].waveCount;

	}

	void SetWaveData()
	{
		EnemyManager* ma = GameManager::GetEnemyManager();
		int random = (int)data_[currentDataIndex].spawnEnemy.size();
		if (random <= 0) return;

		for (int i = 0; i < data_[currentDataIndex].enemyCount; i++) {
			int r = rand() % random;
			ma->SpawnEnemy(data_[currentDataIndex].spawnEnemy.at(r));
		}

	}

	bool IsClearStage()
	{
		if (waveCount_ <= 0) return true;

		return false;
	}

}