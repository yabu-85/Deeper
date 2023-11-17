#pragma once
#include "BehaviourNode.h"

class EnemyBase;
class Player;

class IsPlayerInRangeNode : public Condition
{
	bool isInPlayer_;		//範囲内か外でchildを呼ぶか決める
	float range_;
	EnemyBase* pEnemyBase_;
	Player* pPlayer_;

public:
	IsPlayerInRangeNode(float range, TreeNode* child, bool flag, EnemyBase* enemy, Player* player);
	Status Update() override;
};


