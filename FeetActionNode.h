#pragma once
#include "BehaviourNode.h"

class FeetMove : public BT::Action
{
public:
	FeetMove(std::string name);
	BT::Status Update() override;

};

