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

};

