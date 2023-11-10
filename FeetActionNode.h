#pragma once
#include "BehaviourNode.h"

class Feet;

class FeetMove : public Action
{
	Feet* pFeet_;
public:
	FeetMove(std::string name, Feet* owner);
	Status Update() override;
};

class FeetJump : public Action
{
	Feet* pFeet_;
public:
	FeetJump(std::string name, Feet* owner);
	Status Update() override;
};

class FeetCondition1 : public Condition
{
	Feet* pFeet_;
public:
	FeetCondition1(std::string name, Feet* owner);
	Status Update() override;
};