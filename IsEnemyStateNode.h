#pragma once
#include "BehaviourNode.h"

class EnemyBase;

class IsCombatState : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsCombatState(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};

