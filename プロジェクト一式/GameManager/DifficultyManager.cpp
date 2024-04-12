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
	} 
	const data_[ENEMY_MAX]{
		EnemyData(10),	//Stone
		EnemyData(5),	//Throw
		EnemyData(8),	//Melee
		EnemyData(100),	//SwordBoss
	};
	const float WAVE_DIFFICULTY_SUPPRESS = 0.7f;
	const float LIFE_DIFFICULTY_SUPPRESS = 0.3f;

	int maxDifficulty_ = 0;
	int currentDifficulty_ = 0;

	//難易度を計算する
	void CalcCurrentDifficulty();
	void CalcMaxDifficulty();

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
	CalcMaxDifficulty();
	CalcCurrentDifficulty();

	//ライフの値
	float lifePar = 1.0f - LifeManager::GetLifeParcent();
	currentDifficulty_ = currentDifficulty_ + (int)((float)currentDifficulty_ * LIFE_DIFFICULTY_SUPPRESS * lifePar);

}

void DifficultyManager::SceneTransitionInitialize()
{
	currentDifficulty_ = 0;
	maxDifficulty_ = 0;
}

bool DifficultyManager::AttackPermission()
{
	if (currentDifficulty_ <= maxDifficulty_) return true;
	return false;
}

void DifficultyManager::CalcCurrentDifficulty()
{
	currentDifficulty_ = 0;
	std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
	if (!eList.empty()) {
		for (auto e : eList) {
			if (e->GetStateManager()->GetName() == "Combat" && e->GetCombatStateManager()->GetName() == "Attack")
				currentDifficulty_ += data_[(int)e->GetEnemyType()].enemyPowerLevels;
		}
	}
}

void DifficultyManager::CalcMaxDifficulty()
{
	maxDifficulty_ = 0;
	int maxPower = 99999;
	for (auto e : GameManager::GetEnemyManager()->GetAllEnemy()) {
		int p = data_[e->GetEnemyType()].enemyPowerLevels;
		maxDifficulty_ += p;
		if (p < maxPower) maxPower = p;
	}

	maxDifficulty_ = (int)((float)maxDifficulty_ * WAVE_DIFFICULTY_SUPPRESS);
	if (maxDifficulty_ < maxPower) maxDifficulty_ = maxPower;
}