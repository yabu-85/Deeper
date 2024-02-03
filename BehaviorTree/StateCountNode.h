#pragma once
#include <string>
#include <vector>
#include "BehaviourNode.h"

using std::vector;
using std::string;

class StateCountNode : public Condition
{
	vector<string> stateName_;
	int countThreshold_;	//Count‚Ì‚µ‚«‚¢’l
public:
	StateCountNode(TreeNode* child, int count, vector<string> name);
	Status Update() override;
};

class CombatStateCountNode : public Condition
{
	vector<string> stateName_;
	int countThreshold_;	//Count‚Ì‚µ‚«‚¢’l
public:
	CombatStateCountNode(TreeNode* child, int count, vector<string> name);
	Status Update() override;
};