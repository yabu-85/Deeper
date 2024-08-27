#pragma once
#include "BehaviourNode.h"

class EnemyBase;

//�U���̃N�[���_�E�����I����Ă���Ȃ�Success��Ԃ�
class IsEnemyAttackReady : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyAttackReady(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};

//�A�N�V�����̃N�[���_�E�����I����Ă���Ȃ�Success��Ԃ�
class IsEnemyActionReady : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyActionReady(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};