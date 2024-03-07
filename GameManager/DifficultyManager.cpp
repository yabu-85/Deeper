#include "DifficultyManager.h"
#include "GameManager.h"
#include "../Player/LifeManager.h"
#include "../Enemy/EnemyBase.h"
#include "../State/StateManager.h"
#include <vector>

namespace DifficultyManager {
	struct EnemyData
	{
		int enemyPowerLevels;
		EnemyData(int p) : enemyPowerLevels(p) {}
	} data_[ENEMY_MAX]{
		EnemyData(10),	//Stone
		EnemyData(5),	//Throw
		EnemyData(8),	//Melee
	};
	const float WAVE_DIFFICULTY_SUPPRESS = 0.7f;
	const float LIFE_DIFFICULTY_SUPPRESS = 0.3f;

	int maxDifficulty_ = 0;
	int waveDifficulty_ = 0;

	/*
	難易度を決めるパラメータあるとしたら
		ゲーム周りの敵の強さの合計で判断
		今攻撃している敵のなんか
		クリア時間
		HP
	*/

}

void DifficultyManager::Initialize()
{
}

void DifficultyManager::Update()
{
	//現在の難易度を計算
	waveDifficulty_ = 0;
	std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
	if (!eList.empty()) {
		for (auto e : eList) {
			if (e->GetStateManager()->GetName() == "Combat" && e->GetCombatStateManager()->GetName() == "Attack")
				waveDifficulty_ += data_[(int)e->GetEnemyType()].enemyPowerLevels;
		}
	}

	float lifePar = 1.0f - LifeManager::GetLifeParcent();
	waveDifficulty_ = waveDifficulty_ + (int)((float)waveDifficulty_ * LIFE_DIFFICULTY_SUPPRESS * lifePar);

	//Updateで計算するようにした
	maxDifficulty_ = 0;
	int maxPower = 99999;
	for (auto e : eList) {
		int p = data_[e->GetEnemyType()].enemyPowerLevels;
		maxDifficulty_ += p;
		if (p < maxPower) maxPower = p;
	}

	maxDifficulty_ = (int)((float)maxDifficulty_ * WAVE_DIFFICULTY_SUPPRESS);
	if (maxDifficulty_ < maxPower) maxDifficulty_ = maxPower;

}

void DifficultyManager::SceneTransitionInitialize()
{
	waveDifficulty_ = 0;
}

bool DifficultyManager::AttackPermission()
{
	if (waveDifficulty_ <= maxDifficulty_) return true;
	return false;
}

void DifficultyManager::SetMaxDifficulty(std::vector<ENEMY_TYPE> elist)
{
	maxDifficulty_ = 0;
	int maxPower = 99999;
	for (auto e : elist) {
		int p = data_[e].enemyPowerLevels;
		maxDifficulty_ += p;
		if (p < maxPower) maxPower = p;
	}

	maxDifficulty_ = (int)((float)maxDifficulty_ * WAVE_DIFFICULTY_SUPPRESS);
	if (maxDifficulty_ < maxPower) maxDifficulty_ = maxPower;

}
