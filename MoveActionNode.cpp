#include "MoveActionNode.h"
#include "EnemyBase.h"
#include "StateManager.h"

MoveTarget::MoveTarget(EnemyBase* owner) : Action(), owner_(owner) {}

MoveTarget::Status MoveTarget::Update()
{
	if(owner_->GetCombatStateManager()->GetName() != "Move")
	owner_->GetCombatStateManager()->ChangeState("Move");

	return Status::SUCCESS;
}