#pragma once
#include "../Enemy/EnemyManager.h"
#include <vector>

class EnemyBase;

/// <summary>
/// 今の難易度（危険度）を管理・計算する
/// </summary>
namespace DifficultyManager
{
	void Initialize();
	void Update();
	void SceneTransitionInitialize();
	
	bool AttackPermission();

};

