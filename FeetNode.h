#pragma once
#include "BehaviourNode.h"

class Feet;

class FeetNormalAttack : public Action
{
	Feet* owner_;
public:
	FeetNormalAttack(Feet* owner);
	Status Update() override;
};

class IsNormalAttackState : public Condition
{
	bool isInAttack_;		//Combat‚©‚»‚êˆÈŠO‚©‚ðŒˆ‚ß‚é
	Feet* owner_;
public:
	IsNormalAttackState(TreeNode* child, Feet* enemy, bool flag);
	Status Update() override;
};

