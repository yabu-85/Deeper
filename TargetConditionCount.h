#pragma once
#include "BehaviourNode.h"

//指定したしきい値よりCombat状態のEnemyが少なければ(以下)Succes
class CombatEnemyCount : public Condition
{
	int countThreshold_;	//Countのしきい値
public:
	CombatEnemyCount(int count, TreeNode* child);
	Status Update() override;
};