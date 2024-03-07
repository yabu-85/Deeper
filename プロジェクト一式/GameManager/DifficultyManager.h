#pragma once
#include "../Enemy/EnemyManager.h"
#include <vector>

class EnemyBase;

namespace DifficultyManager
{
	void Initialize();
	void Update();
	void SceneTransitionInitialize();
	
	bool AttackPermission();
	void SetMaxDifficulty(std::vector<ENEMY_TYPE> elist);

};

