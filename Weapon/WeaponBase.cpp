#include "WeaponBase.h"
#include "../State/StateManager.h"

WeaponBase::WeaponBase(GameObject* parent, std::string name)
	: GameObject(parent, name), hModel_(-1), pStateManager_(nullptr), atkEnd_(true), durance_(0), boneIndex_(-1), partIndex_(-1)
{
    offsetTrans_.scale_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void WeaponBase::CalcOffset(Transform& trans)
{
    trans.position_.x += offsetTrans_.position_.x;
    trans.position_.y += offsetTrans_.position_.y;
    trans.position_.z += offsetTrans_.position_.z;

    trans.rotate_.x += offsetTrans_.rotate_.x;
    trans.rotate_.y += offsetTrans_.rotate_.y;
    trans.rotate_.z += offsetTrans_.rotate_.z;

    trans.scale_.x += offsetTrans_.scale_.x;
    trans.scale_.y += offsetTrans_.scale_.y;
    trans.scale_.z += offsetTrans_.scale_.z;
}

void WeaponBase::UpdateState()
{
	pStateManager_->Update();
}

bool WeaponBase::IsBlockend()
{
    if (durance_ <= 0) return true;
    return false;
}
