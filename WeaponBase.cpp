#include "WeaponBase.h"
#include "StateManager.h"

WeaponBase::WeaponBase(GameObject* parent)
	: hModel_(-1), offsetPosition_(0.0f, 0.0f, 0.0f), offsetRotation_(0.0f, 0.0f, 0.0f), pStateManager_(nullptr), atkEnd_(true)
{
}

void WeaponBase::CalcOffset()
{
    transform_.position_ = GetParent()->GetPosition();
    transform_.position_.x += offsetPosition_.x;
    transform_.position_.y += offsetPosition_.y;
    transform_.position_.z += offsetPosition_.z;

    transform_.rotate_.x += offsetRotation_.x;
    transform_.rotate_.y += offsetRotation_.y;
    transform_.rotate_.z += offsetRotation_.z;
}

void WeaponBase::UpdateState()
{
	pStateManager_->Update();
}