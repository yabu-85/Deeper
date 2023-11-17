#include "IsEnemyStateNode.h"
#include "EnemyBase.h"
#include "StateManager.h"

IsCombatState::IsCombatState(TreeNode* child, EnemyBase* enemy) : Condition(child), pEnemyBase_(enemy)
{
}

IsCombatState::Status IsCombatState::Update()
{
	if (pEnemyBase_->GetStateManager()->GetName() == "Combat") {
		return child_->Tick();
	}

	return Status::FAILURE;
}
