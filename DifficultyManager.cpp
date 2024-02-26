#include "DifficultyManager.h"
#include "GameManager.h"
#include "Enemy/EnemyBase.h"
#include "State/StateManager.h"
#include <vector>

namespace DifficultyManager {
	struct EnemyData
	{
		int enemyPowerLevels;
		EnemyData(int p) : enemyPowerLevels(p) {}
	}data_[ENEMY_MAX]{
		EnemyData(10),	//Stone
		EnemyData(5),	//Throw
	};
	const float DIFFICULTY_SUPPRESS = 0.8f;

	//------------------------------------------------------

	int maxDifficulty_ = 50;
	int waveDifficulty_ = 0;

	/*
	難易度を決めるパラメータ
	　Wave　
		クリア時間
		HP
	　常時
		ゲーム周りの敵の強さの合計で判断
		今攻撃している敵のなんか
		HP
	*/

	//--------------------------------------------

	void Initialize()
	{
	}

	void Update()
	{
		//現在の難易度を計算
		waveDifficulty_ = 0;
		std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
		if (!eList.empty()) {
			for (auto e : eList) {
				if (e->GetStateManager()->GetName() != "Combat" && e->GetCombatStateManager()->GetName() == "Attack")
					waveDifficulty_ += data_[(int)e->GetEnemyType()].enemyPowerLevels;
			}
		}

	}

	void SceneTransitionInitialize()
	{
		waveDifficulty_ = 0;
	}

	bool AttackPermission()
	{
		if (waveDifficulty_ <= maxDifficulty_) return true;
		return false;
	}

	void SetMaxDifficulty(std::vector<ENEMY_TYPE> elist)
	{
		maxDifficulty_ = 99999;
		int maxPower = 0;
		for (auto e : elist) {
			int p = data_[e].enemyPowerLevels;
			maxDifficulty_ += p;
			if (p < maxPower) maxPower = p;
		}
		
		maxDifficulty_ = (int)((float)maxDifficulty_ * DIFFICULTY_SUPPRESS);
		if (maxDifficulty_ < maxPower) maxDifficulty_ = maxPower;
	
	}

}

