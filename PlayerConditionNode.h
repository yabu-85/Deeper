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
	IsPlayerInRangeNode(float range, TreeNode* child, EnemyBase* enemy, Player* player);
	Status Update() override;
};


