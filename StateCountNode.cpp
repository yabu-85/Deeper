#include "StateCountNode.h"
#include "EnemyBase.h"
#include "EnemySpawnCtrl.h"
#include "StateManager.h"
#include "GameManager.h"
#include <vector>

AttackStateCountNode::AttackStateCountNode(int count, TreeNode* child) : Condition(child), countThreshold_(count)
{
}

AttackStateCountNode::Status AttackStateCountNode::Update()
{
	std::vector<EnemyBase*> eneList = GameManager::GetEnemySpawnCtrl()->GetAllEnemy();
	int con = 0;
	for (auto e : eneList) {
		if (e->GetCombatStateManager()->GetName() == "Attack") con++;
	}

	if (countThreshold_ >= con) return child_->Tick();
	return Status::FAILURE;
}