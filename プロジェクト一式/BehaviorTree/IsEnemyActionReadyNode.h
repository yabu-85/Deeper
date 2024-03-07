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

class IsEnemyActionReady : public Condition
{
	EnemyBase* pEnemyBase_;

public:
	IsEnemyActionReady(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};