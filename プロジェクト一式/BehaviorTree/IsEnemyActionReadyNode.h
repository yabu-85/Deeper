#pragma once
#include "BehaviourNode.h"

class EnemyBase;

//攻撃のクールダウンが終わっているならSuccessを返す
class IsEnemyAttackReady : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyAttackReady(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};

//アクションのクールダウンが終わっているならSuccessを返す
class IsEnemyActionReady : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyActionReady(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};