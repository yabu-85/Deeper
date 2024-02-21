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

	//ステージごとにEnemyTableを分けて、そのテーブルの中からランダムで選ぶ
	static const std::vector<std::vector<ENEMY_TYPE>> spawnEnemyTable[2] = {
		{
			{ ENEMY_STONEGOLEM, ENEMY_STONEGOLEM, ENEMY_THROW},
			{ ENEMY_STONEGOLEM, ENEMY_THROW, ENEMY_THROW},
			{ ENEMY_STONEGOLEM, ENEMY_STONEGOLEM, ENEMY_THROW, ENEMY_THROW},
		},
		{
			{ ENEMY_MAX, ENEMY_MAX},
			{ ENEMY_MAX, ENEMY_MAX},
			{ ENEMY_MAX, ENEMY_MAX},
		}
	};

	struct StageData {
		int waveCount;		//Waveの数
		int spawnTable;		//spawnEnemyTableを選ぶための

		StageData(int wave, int list) : waveCount(wave), spawnTable(list) {};
	} 
	data_[MAX] = {
		StageData(0, 0),
		StageData(3, 0),
		StageData(1, 0),
	};

	bool isStageClear = false;
	int currentDataIndex = 0;
	int waveCount_ = 0;

	//-----------------------------------------------------------------

	void Update()
	{
		//初期・Waveクリア
		if (GameManager::GetEnemyManager()->IsEnemyListEmpty()) {
			if (waveCount_ <= 0) {
				isStageClear = true;
			}
			else if (waveCount_ >= 1) {
				waveCount_--;
				SetWaveData();
			}
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
		//spawnEnemyTableの中からspawnTableの場所からとってそのvectorのsizeを取得
		int randMax = (int)spawnEnemyTable[data_[currentDataIndex].spawnTable].size();
		if (randMax <= 0) return;
		int r = rand() % randMax;

		EnemyManager* ma = GameManager::GetEnemyManager();
		int max = (int)spawnEnemyTable[data_[currentDataIndex].spawnTable].size();
		for (int i = 0; i < max; i++) {
			ENEMY_TYPE t = spawnEnemyTable[data_[currentDataIndex].spawnTable][r].at(i);
			ma->SpawnEnemy(t);
		}
	}

	bool IsClearStage()
	{
		return isStageClear;
	}

	void SceneTransitionInitialize()
	{
		SetStageData();
		isStageClear = false;

	}

}