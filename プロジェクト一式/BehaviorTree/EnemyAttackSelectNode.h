#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;

class EnemyAttackSelectNode : public Action
{
	EnemyBase* owner_;
public:
	EnemyAttackSelectNode(EnemyBase* owner);
	Status Update() override;
};