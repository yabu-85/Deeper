#include "IsEnemyAttackPermission.h"
#include "../Enemy/EnemyBase.h"
#include "../DifficultyManager.h"

IsEnemyAttackPermission::IsEnemyAttackPermission(TreeNode* child, EnemyBase* enemy)
	: Condition(child), pEnemyBase_(enemy)
{
}

IsEnemyAttackPermission::Status IsEnemyAttackPermission::Update()
{
	if (DifficultyManager::IsEnemyAttackPermission()) return child_->Tick();
	return Status::FAILURE;
}