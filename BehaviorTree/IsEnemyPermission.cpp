#include "IsEnemyPermission.h"
#include "../Enemy/EnemyBase.h"
#include "../CombatAI.h"

IsEnemyAttackPermission::IsEnemyAttackPermission(TreeNode* child, EnemyBase* enemy) : Condition(child), pEnemyBase_(enemy)
{
}

IsEnemyAttackPermission::Status IsEnemyAttackPermission::Update()
{
	if (CombatAI::IsEnemyAttackPermission(pEnemyBase_)) return child_->Tick();
	return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------

IsEnemyMovePermission::IsEnemyMovePermission(TreeNode* child, EnemyBase* enemy) : Condition(child), pEnemyBase_(enemy)
{
}

IsEnemyMovePermission::Status IsEnemyMovePermission::Update()
{
	if (CombatAI::IsEnemyMovePermission(pEnemyBase_)) return child_->Tick();
	return Status::FAILURE;
}