#pragma once
#include "BehaviourNode.h"

class AttackStateCountNode : public Condition
{
	int countThreshold_;	//Count‚Ì‚µ‚«‚¢’l
public:
	AttackStateCountNode(int count, TreeNode* child);
	Status Update() override;
};

