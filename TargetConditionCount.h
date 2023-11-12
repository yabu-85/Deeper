#pragma once
#include "BehaviourNode.h"

//w’è‚µ‚½‚µ‚«‚¢’l‚æ‚èTargetó‘Ô‚ÌEnemy‚ª­‚È‚¯‚ê‚Î(ˆÈ‰º)Succes
class TargetConditionCount : public Condition
{
	int countThreshold_;	//Targetó‘ÔCount‚Ì‚µ‚«‚¢’l
public:
	TargetConditionCount(int count, TreeNode* child);
	Status Update() override;
};