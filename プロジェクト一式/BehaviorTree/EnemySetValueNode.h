#pragma once
#include "BehaviourNode.h"
class EnemyBase;

class EnemySetAttackCoolDown : public Action
{
	int time_;
	EnemyBase* owner_;
public:
	EnemySetAttackCoolDown(EnemyBase* owner, int t);
	Status Update() override;
};