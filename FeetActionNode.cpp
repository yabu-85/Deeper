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
	: Action(name), pFeet_(owner), time_(0)
{
}

FeetJump::Status FeetJump::Update()
{
	time_++;
	XMFLOAT3 pos = pFeet_->GetPosition();
	pFeet_->SetPosition(XMFLOAT3(pos.x, pos.y + 0.1f, pos.z));
	if (time_ >= 60) {
		pFeet_->SetPosition(XMFLOAT3(50.0f, 0.0f, 50.0f));
		return Status::INVALID;
	}

	return Status::RUNNING;
}

void FeetJump::Initialize()
{
	time_ = 0;
}
