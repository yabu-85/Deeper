#pragma once
#include "BehaviourNode.h"

//w’è‚µ‚½‚µ‚«‚¢’l‚æ‚èCombató‘Ô‚ÌEnemy‚ª­‚È‚¯‚ê‚Î(ˆÈ‰º)Succes
class CombatEnemyCount : public Condition
{
	int countThreshold_;	//Count‚Ì‚µ‚«‚¢’l
public:
	CombatEnemyCount(int count, TreeNode* child);
	Status Update() override;
};