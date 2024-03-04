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
	}data_[ENEMY_MAX]{
		EnemyData(10),	//Stone
		EnemyData(5),	//Throw
		EnemyData(8),	//Melee
	};
	const float WAVE_DIFFICULTY_SUPPRESS = 0.7f;
	const float LIFE_DIFFICULTY_SUPPRESS = 0.3f;
	
	//------------------------------------------------------

	int maxDifficulty_ = 0;
	int waveDifficulty_ = 0;

	/*
	��Փx�����߂�p�����[�^����Ƃ�����
		�Q�[������̓G�̋����̍��v�Ŕ��f
		���U�����Ă���G�̂Ȃ�
		�N���A����
		HP
	*/

	//--------------------------------------------

	void Initialize()
	{
	}

	void Update()
	{
		//���݂̓�Փx���v�Z
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

		//Update�Ōv�Z����悤�ɂ���
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

}

