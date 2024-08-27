#pragma once
#include "BehaviourNode.h"

class EnemyBase;

//敵の原点がスクリーン内にあるならSuccessを返す
class IsEnemyWithinScreen : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyWithinScreen(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};