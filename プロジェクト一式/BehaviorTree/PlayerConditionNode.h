#pragma once
#include "BehaviourNode.h"

class EnemyBase;
class Player;

//�v���C���[�Ƃ̋����� n �ȉ��Ȃ�
class IsPlayerInRangeNode : public Condition
{
	float range_;
	EnemyBase* pEnemyBase_;
	Player* pPlayer_;
public:
	IsPlayerInRangeNode(TreeNode* child, float range, EnemyBase* enemy, Player* player);
	Status Update() override;
};

//�v���C���[�Ƃ̋����� n �ȏ�Ȃ�
class IsPlayerNotInRangeNode : public Condition
{
	float range_;
	EnemyBase* pEnemyBase_;
	Player* pPlayer_;
public:
	IsPlayerNotInRangeNode(TreeNode* child, float range, EnemyBase* enemy, Player* player);
	Status Update() override;
};
