#pragma once
#include "BehaviourNode.h"

class EnemyBase;

class IsEnemyAttackReady : public Condition
{
	EnemyBase* pEnemyBase_;
	
public:
	IsEnemyAttackReady(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};