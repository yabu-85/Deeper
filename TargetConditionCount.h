#pragma once
#include "BehaviourNode.h"

class TargetConditionCount : public Condition
{
	int countThreshold_;	//Target���Count�̂������l
public:
	TargetConditionCount(int count);
	Status Update() override;
};

