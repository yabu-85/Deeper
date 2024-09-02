#pragma once
#include <string>
#include <vector>
#include "BehaviourNode.h"

using std::vector;
using std::string;

//指定したStateの敵が指定した数いたら
class StateCountNode : public Condition
{
	vector<string> stateName_;
	int countThreshold_;	//Countのしきい値
public:
	StateCountNode(TreeNode* child, int count, vector<string> name);
	Status Update() override;
};

//指定したStateの敵が指定した数いなかったら
class CombatStateCountNode : public Condition
{
	vector<string> stateName_;
	int countThreshold_;	//Countのしきい値
public:
	CombatStateCountNode(TreeNode* child, int count, vector<string> name);
	Status Update() override;
};