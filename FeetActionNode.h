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
	int time_;
	Feet* pFeet_;
public:
	FeetJump(std::string name, Feet* owner);
	Status Update() override;
	void Initialize() override;
};