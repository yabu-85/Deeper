#pragma once
#include "BehaviourNode.h"

class EnemyBase;

class IsAttackState : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsAttackState(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};

