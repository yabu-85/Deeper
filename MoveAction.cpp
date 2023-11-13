#include "MoveAction.h"
#include "EnemyBase.h"

MoveTarget::MoveTarget(EnemyBase* owner, float speed, float range) : Action(), owner_(owner) {}

MoveTarget::Status MoveTarget::Update()
{
	if (owner_->Move()) {
		return Status::SUCCESS;
	}

	return Status::RUNNING;
}