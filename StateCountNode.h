#pragma once
#include <string>
#include "BehaviourNode.h"

class StateCountNode : public Condition
{
	std::string stateName_;
	int countThreshold_;	//Countのしきい値
public:
	StateCountNode(TreeNode* child, int count, std::string name);
	Status Update() override;
};

class CombatStateCountNode : public Condition
{
	std::string stateName_;
	int countThreshold_;	//Countのしきい値
public:
	CombatStateCountNode(TreeNode* child, int count, std::string name);
	Status Update() override;
};