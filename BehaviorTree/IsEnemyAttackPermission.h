#pragma once
#include "BehaviourNode.h"

class EnemyBase;

class IsEnemyAttackPermission : public Condition
{
	EnemyBase* pEnemyBase_;
	
public:
	IsEnemyAttackPermission(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};

