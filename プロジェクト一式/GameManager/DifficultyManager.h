#pragma once
#include "../Enemy/EnemyManager.h"
#include <vector>

class EnemyBase;

/// <summary>
/// ���̓�Փx�i�댯�x�j���Ǘ��E�v�Z����
/// </summary>
namespace DifficultyManager
{
	void Initialize();
	void Update();
	void SceneTransitionInitialize();
	
	bool AttackPermission();

};

