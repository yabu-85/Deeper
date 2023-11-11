#pragma once
#include "BehaviourNode.h"

class TargetConditionCount : public Condition
{
	int countThreshold_;	//Target状態Countのしきい値
public:
	TargetConditionCount(int count, TreeNode* child);
	Status Update() override;
};