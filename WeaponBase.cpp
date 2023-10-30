#include "WeaponBase.h"
#include "StateManager.h"

WeaponBase::WeaponBase(GameObject* parent)
	: hModel_(-1), offsetPosition_(0.0f, 0.0f, 0.0f), offsetRotation_(0.0f, 0.0f, 0.0f), pStateManager_(nullptr), atkEnd_(true)
{
}

void WeaponBase::UpdateState()
{
	pStateManager_->Update();
}