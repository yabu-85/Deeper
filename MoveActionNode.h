#pragma once
#include "BehaviourNode.h"

class EnemyBase;

class MoveTarget : public Action
{
	EnemyBase* owner_;
public:
	MoveTarget(EnemyBase* owner);
	Status Update() override;
};