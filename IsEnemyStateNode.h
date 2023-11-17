#pragma once
#include "BehaviourNode.h"

class EnemyBase;

class IsCombatState : public Condition
{
	bool isInCombat_;		//Combatかそれ以外かを決める
	EnemyBase* pEnemyBase_;
public:
	IsCombatState(TreeNode* child, EnemyBase* enemy, bool flag);
	Status Update() override;
};

