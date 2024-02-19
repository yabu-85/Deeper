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
		int waveCount;		//Wave‚Ì”
		int enemyCount;		//‚PWave‚Åo‚Ä‚­‚é“G‚Ì”
		std::vector<ENEMY_TYPE> spawnEnemy;
		
		StageData(int wave, std::vector<ENEMY_TYPE> list, int enemy) : waveCount(wave), spawnEnemy(list), enemyCount(enemy) 
		{
		};
	} 
	datas[MAX] = {
		StageData(0, {}, 0 ),
		StageData(1, { ENEMY_STONEGOLEM, ENEMY_THROW }, 3),
		StageData(1, { ENEMY_STONEGOLEM, ENEMY_THROW }, 3),
	};

	int currentDataIndex = 0;

	//-----------------------------------------------------------------

	void SetStageData() {
		SCENE_ID cs = GameManager::GetSceneManager()->GetSceneID();
		if (cs == SCENE_ID_STAGE1) currentDataIndex = 0;
		else if (cs == SCENE_ID_STAGE2) currentDataIndex = 1;
		else if (cs == SCENE_ID_STAGE3) currentDataIndex = 2;

	}

	void ClearWave() {

	}

}