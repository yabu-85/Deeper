#pragma once
#include "BehaviourNode.h"

class Feet;

class FeetJump : public Action
{
	Feet* pFeet_;
public:
	FeetJump(Feet* owner);
	Status Update() override;
};

class FeetCondition1 : public Condition
{
	Feet* pFeet_;
public:
	FeetCondition1(Feet* owner);
	Status Update() override;
};