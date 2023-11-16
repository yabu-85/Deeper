#include "TargetConditionCountNode.h"
#include "EnemyBase.h"
#include "EnemySpawnCtrl.h"
#include "StateManager.h"
#include "GameManager.h"
#include <vector>

CombatEnemyCount::CombatEnemyCount(int count, TreeNode* child) : Condition(child), countThreshold_(count)
{
}

CombatEnemyCount::Status CombatEnemyCount::Update()
{
	std::vector<EnemyBase*> eneList = GameManager::GetEnemySpawnCtrl()->GetAllEnemy();
	int con = 0;
	for (auto e : eneList) {
		if (e->GetStateManager()->GetName() == "Combat") con++;
	}

	if (countThreshold_ >= con) return child_->Tick();
	return Status::FAILURE;
}