#include "MoveActionNode.h"
#include "EnemyBase.h"
#include "StateManager.h"

MoveTarget::MoveTarget(EnemyBase* owner) : Action(), owner_(owner) {}

MoveTarget::Status MoveTarget::Update()
{
	owner_->GetCombatStateManager()->ChangeState("Move");
	return Status::SUCCESS;
}