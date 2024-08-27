#pragma once
#include "BehaviourNode.h"
class EnemyBase;

//AttackCoolDownのセットを行う、必ずSuccessが返ってくる
class EnemySetAttackCoolDown : public Action
{
	int time_;
	EnemyBase* owner_;
public:
	EnemySetAttackCoolDown(EnemyBase* owner, int t);
	Status Update() override;
};