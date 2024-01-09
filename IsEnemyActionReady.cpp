#include "IsEnemyActionReady.h"
#include "EnemyBase.h"

IsEnemyAttackReady::IsEnemyAttackReady(TreeNode* child, EnemyBase* enemy)
	: Condition(child), pEnemyBase_(enemy)
{
}

IsEnemyAttackReady::Status IsEnemyAttackReady::Update()
{
	if (pEnemyBase_->IsAttackReady()) return child_->Tick();
	return Status::FAILURE;
}