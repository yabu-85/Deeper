#pragma once
#include "BehaviourNode.h"

//指定したしきい値よりTarget状態のEnemyが少なければ(以下)Succes
class TargetConditionCount : public Condition
{
	int countThreshold_;	//Target状態Countのしきい値
public:
	TargetConditionCount(int count, TreeNode* child);
	Status Update() override;
};