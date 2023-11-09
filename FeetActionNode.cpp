#include "FeetActionNode.h"

FeetMove::FeetMove(std::string name)
	: BT::Action(name)
{
}

BT::Status FeetMove::Update()
{
	return BT::Status();
}
