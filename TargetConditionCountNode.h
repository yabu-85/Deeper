#pragma once
#include "BehaviourNode.h"

//�w�肵���������l���Combat��Ԃ�Enemy�����Ȃ����(�ȉ�)Succes
class CombatEnemyCount : public Condition
{
	int countThreshold_;	//Count�̂������l
public:
	CombatEnemyCount(int count, TreeNode* child);
	Status Update() override;
};