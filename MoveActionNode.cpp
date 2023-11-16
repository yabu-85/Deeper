#include "MoveActionNode.h"
#include "EnemyBase.h"
#include "StateManager.h"

MoveTarget::MoveTarget(EnemyBase* owner, float speed, float range) : Action(), owner_(owner) {}

MoveTarget::Status MoveTarget::Update()
{
	if (owner_->GetCombatStateManager()->GetName() == "Move") {
		return Status::SUCCESS;
	}

	return Status::RUNNING;
}