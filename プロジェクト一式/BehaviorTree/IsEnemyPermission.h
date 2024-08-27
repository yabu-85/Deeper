#pragma once
#include "BehaviourNode.h"

class EnemyBase;

//CombatAIŒo—R‚ÅUŒ‚‹–‰Â‚ª~‚è‚½‚çSuccess‚ğ•Ô‚·
class IsEnemyAttackPermission : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyAttackPermission(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};

//CombatAIŒo—R‚ÅˆÚ“®‹–‰Â‚ª~‚è‚½‚çSuccess‚ğ•Ô‚·
class IsEnemyMovePermission : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyMovePermission(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};