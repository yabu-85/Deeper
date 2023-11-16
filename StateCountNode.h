#pragma once
#include "BehaviourNode.h"

class AttackStateCountNode : public Condition
{
	int countThreshold_;	//Countのしきい値
public:
	AttackStateCountNode(int count, TreeNode* child);
	Status Update() override;
};

