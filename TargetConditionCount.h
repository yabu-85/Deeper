#pragma once
#include "BehaviourNode.h"

//�w�肵���������l���Target��Ԃ�Enemy�����Ȃ����(�ȉ�)Succes
class TargetConditionCount : public Condition
{
	int countThreshold_;	//Target���Count�̂������l
public:
	TargetConditionCount(int count, TreeNode* child);
	Status Update() override;
};