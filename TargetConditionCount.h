#pragma once
#include "BehaviourNode.h"

class TargetConditionCount : public Condition
{
	int countThreshold_;	//Targetó‘ÔCount‚Ì‚µ‚«‚¢’l
public:
	TargetConditionCount(int count, TreeNode* child);
	Status Update() override;
};