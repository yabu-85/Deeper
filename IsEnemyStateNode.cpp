#include "IsEnemyStateNode.h"
#include "EnemyBase.h"
#include "StateManager.h"

IsAttackState::IsAttackState(TreeNode* child, EnemyBase* enemy) : Condition(child), pEnemyBase_(enemy)
{
}

IsAttackState::Status IsAttackState::Update()
{
	if (pEnemyBase_->GetStateManager()->GetName() == "Combat") {
		return child_->Tick();
	}

	return Status::FAILURE;
}
