#include "FeetActionNode.h"
#include "Feet.h"

//--------------------------Jump--------------------------

FeetJump::FeetJump(Feet* owner) : Action(), pFeet_(owner) {}

FeetJump::Status FeetJump::Update()
{
	pFeet_->SetPosition(XMFLOAT3(50.0f, 0.0f, 50.0f));
	return Status::SUCCESS;
}

//--------------------------FeetCondition1--------------------------

FeetCondition1::FeetCondition1(Feet* owner, TreeNode* child) : Condition(child), pFeet_(owner) {}

FeetCondition1::Status FeetCondition1::Update()
{
	if (rand() % 600 == 0) {
		return child_->Tick();
	}
	return Status::FAILURE;
}