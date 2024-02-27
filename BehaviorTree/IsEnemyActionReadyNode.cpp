#include "IsEnemyActionReadyNode.h"
#include "../Enemy/EnemyBase.h"

IsEnemyAttackReady::IsEnemyAttackReady(TreeNode* child, EnemyBase* enemy) : Condition(child), pEnemyBase_(enemy)
{
}

IsEnemyAttackReady::Status IsEnemyAttackReady::Update()
{
	if (pEnemyBase_->IsAttackReady()) return child_->Tick();
	return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------

IsEnemyActionReady::IsEnemyActionReady(TreeNode* child, EnemyBase* enemy) : Condition(child), pEnemyBase_(enemy)
{
}

IsEnemyActionReady::Status IsEnemyActionReady::Update()
{
	if (pEnemyBase_->GetActionCoolDown() <= 0) return child_->Tick();
	return Status::FAILURE;
}
