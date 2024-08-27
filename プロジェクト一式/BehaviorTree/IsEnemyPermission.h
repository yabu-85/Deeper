#pragma once
#include "BehaviourNode.h"

class EnemyBase;

//CombatAI�o�R�ōU�������~�肽��Success��Ԃ�
class IsEnemyAttackPermission : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyAttackPermission(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};

//CombatAI�o�R�ňړ������~�肽��Success��Ԃ�
class IsEnemyMovePermission : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyMovePermission(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};