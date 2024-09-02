#pragma once
#include <string>
#include <vector>
#include "BehaviourNode.h"

using std::vector;
using std::string;

//�w�肵��State�̓G���w�肵����������
class StateCountNode : public Condition
{
	vector<string> stateName_;
	int countThreshold_;	//Count�̂������l
public:
	StateCountNode(TreeNode* child, int count, vector<string> name);
	Status Update() override;
};

//�w�肵��State�̓G���w�肵�������Ȃ�������
class CombatStateCountNode : public Condition
{
	vector<string> stateName_;
	int countThreshold_;	//Count�̂������l
public:
	CombatStateCountNode(TreeNode* child, int count, vector<string> name);
	Status Update() override;
};