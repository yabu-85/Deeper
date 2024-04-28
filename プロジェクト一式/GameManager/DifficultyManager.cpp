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
		EnemyData(100),	//Scarecrow
	};
	const float WAVE_DIFFICULTY_SUPPRESS = 0.7f;
	const float LIFE_DIFFICULTY_SUPPRESS = 0.3f;

	int maxDifficulty_ = 0;
	int currentDifficulty_ = 0;

	//��Փx���v�Z����
	void CalcCurrentDifficulty();
	void CalcMaxDifficulty();

	/*
	��Փx�����߂�p�����[�^����Ƃ�����
		�Q�[������̓G�̋����̍��v�Ŕ��f
		���U�����Ă���G�̂Ȃ�
		�N���A����
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

	//���C�t�̒l
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