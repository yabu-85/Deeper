#include "FeetActionNode.h"
#include "Feet.h"

FeetMove::FeetMove(std::string name, Feet* owner)
	: Action(name), pFeet_(owner)
{
}

FeetMove::Status FeetMove::Update()
{
	XMFLOAT3 pos = pFeet_->GetPosition();
	pFeet_->SetPosition(XMFLOAT3(pos.x, pos.y, pos.z + 0.1f));
	return Status::SUCCESS;
}

//--------------------------Jump--------------------------

FeetJump::FeetJump(std::string name, Feet* owner)
	: Action(name), pFeet_(owner)
{
}

FeetJump::Status FeetJump::Update()
{
	pFeet_->SetPosition(XMFLOAT3(50.0f, 0.0f, 50.0f));
	return Status::SUCCESS;
}

//--------------------------FeetCondition1--------------------------

FeetCondition1::FeetCondition1(std::string name, Feet* owner)
	: Condition(name), pFeet_(owner)
{
}

FeetCondition1::Status FeetCondition1::Update()
{
	if (rand() % 600 == 0) {
		return Status::SUCCESS;
	}
	return Status::FAILURE;
}
