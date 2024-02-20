#include "DifficultyManager.h"
#include "GameManager.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/EnemyBase.h"
#include "State/StateManager.h"
#include <vector>

namespace DifficultyManager {
	static const int MAX_DIFFICULTY = 50;
	int difficulty = 0;

	struct EnemyData
	{
		int enemyPowerLevels;
		EnemyData(int p) : enemyPowerLevels(p) {}
	}data_[ENEMY_MAX]{
		EnemyData(10),	//Stone
		EnemyData(5),	//Throw
	};

	void Initialize()
	{

	}

	void Update()
	{
		//“ïˆÕ“x‚ÌŒvŽZ
		difficulty = 0;
		std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
		if (!eList.empty()) {
			for (auto e : eList) {
				if (e->GetStateManager()->GetName() != "Combat") continue;
				
				std::string name = e->GetCombatStateManager()->GetName();
				if (name == "Attack" || name == "Move") 
					difficulty += data_[(int)e->GetEnemyType()].enemyPowerLevels;
			
			}
		}

	}

	void SceneTransitionInitialize()
	{
		difficulty = 0;

	}

	bool IsEnemyAttackPermission()
	{
		if (difficulty <= MAX_DIFFICULTY) return true;
		return false;
	}

}

