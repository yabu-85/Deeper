#include "IsEnemyStateNode.h"
#include "EnemyBase.h"
#include "StateManager.h"

IsCombatState::IsCombatState(TreeNode* child, EnemyBase* enemy, bool flag) : Condition(child), pEnemyBase_(enemy), isInCombat_(flag)
{
}

IsCombatState::Status IsCombatState::Update()
{
	if ((pEnemyBase_->GetStateManager()->GetName() == "Combat") == isInCombat_ ) {
		return child_->Tick();
	}

	return Status::FAILURE;
}
