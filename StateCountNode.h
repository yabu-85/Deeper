#pragma once
#include "BehaviourNode.h"

class AttackStateCountNode : public Condition
{
	int countThreshold_;	//Count�̂������l
public:
	AttackStateCountNode(int count, TreeNode* child);
	Status Update() override;
};

