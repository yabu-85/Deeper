#include "StateCountNode.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyManager.h"
#include "../State/StateManager.h"
#include "../GameManager/GameManager.h"
#include <vector>

StateCountNode::StateCountNode(TreeNode* child, int count, vector<string> name)
	: Condition(child), countThreshold_(count), stateName_(name)
{
}

StateCountNode::Status StateCountNode::Update()
{
	std::vector<EnemyBase*> eneList = GameManager::GetEnemyManager()->GetAllEnemy();
	int con = 0;
	for (auto e : eneList) {
		for (auto it : stateName_) {
			if (e->GetStateManager() && e->GetStateManager()->GetName() == it) con++;
		}
	}

	if (countThreshold_ > con) return child_->Tick();
	return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------

CombatStateCountNode::CombatStateCountNode(TreeNode* child, int count, vector<string> name)
	: Condition(child), countThreshold_(count), stateName_(name)
{
}

CombatStateCountNode::Status CombatStateCountNode::Update()
{
	std::vector<EnemyBase*> eneList = GameManager::GetEnemyManager()->GetAllEnemy();
	int con = 0;
	for (auto e : eneList) {
		for (auto it : stateName_) {
			if (e->GetCombatStateManager() && e->GetCombatStateManager()->GetName() == it) con++;
		}
	}

	if (countThreshold_ > con) return child_->Tick();
	return Status::FAILURE;
}
