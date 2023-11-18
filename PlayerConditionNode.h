#pragma once
#include "BehaviourNode.h"

class EnemyBase;
class Player;

class IsPlayerInRangeNode : public Condition
{
	float range_;
	EnemyBase* pEnemyBase_;
	Player* pPlayer_;

public:
	IsPlayerInRangeNode(TreeNode* child, float range, EnemyBase* enemy, Player* player);
	Status Update() override;
};

class IsPlayerNotInRangeNode : public Condition
{
	float range_;
	EnemyBase* pEnemyBase_;
	Player* pPlayer_;

public:
	IsPlayerNotInRangeNode(TreeNode* child, float range, EnemyBase* enemy, Player* player);
	Status Update() override;
};
