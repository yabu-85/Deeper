#include "EnemySetValueNode.h"
#include "../Enemy/EnemyBase.h"

EnemySetAttackCoolDown::EnemySetAttackCoolDown(EnemyBase* owner, int t)
	: Action(), time_(t), owner_(owner)
{
}

EnemySetAttackCoolDown::Status EnemySetAttackCoolDown::Update()
{
	owner_->SetAttackCoolDown(time_);
	return Status::SUCCESS;
}