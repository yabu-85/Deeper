#pragma once
#include "BehaviourNode.h"

class EnemyBase;

//CombatAI経由で攻撃許可が降りたらSuccessを返す
class IsEnemyAttackPermission : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyAttackPermission(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};

//CombatAI経由で移動許可が降りたらSuccessを返す
class IsEnemyMovePermission : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyMovePermission(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};