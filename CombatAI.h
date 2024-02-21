#pragma once

class EnemyBase;

namespace CombatAI
{
	void Initialize();
	void Update();
	bool IsEnemyAttackPermission(EnemyBase* enemy);

};

