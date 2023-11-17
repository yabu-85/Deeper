#pragma once
#include "BehaviourNode.h"

class EnemyBase;

class IsCombatState : public Condition
{
	bool isInCombat_;		//Combat������ȊO�������߂�
	EnemyBase* pEnemyBase_;
public:
	IsCombatState(TreeNode* child, EnemyBase* enemy, bool flag);
	Status Update() override;
};

