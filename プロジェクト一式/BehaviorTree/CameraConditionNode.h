#pragma once
#include "BehaviourNode.h"

class EnemyBase;

class IsEnemyWithinScreen : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyWithinScreen(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};