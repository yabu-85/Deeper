#include "WeaponMainOrnament.h"
#include "PlayerOrnament.h"
#include "../Engine/Model.h"

namespace {

}

WeaponMainOrnament::WeaponMainOrnament(GameObject* parent)
	: GameObject(parent, "WeaponMainOrnament"), pPlayerOrnament_(nullptr)
{
    transform_.pParent_ = nullptr;
}

WeaponMainOrnament::~WeaponMainOrnament()
{
}

void WeaponMainOrnament::Initialize()
{
    hModel_ = Model::Load("Model/Sword.fbx");
    assert(hModel_ >= 0);

    pPlayerOrnament_ = static_cast<PlayerOrnament*>(GetParent());
    Model::GetBoneIndex(pPlayerOrnament_->GetModelHandle(), "Weapon", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);

}

void WeaponMainOrnament::Update()
{

}

void WeaponMainOrnament::Draw()
{
    wandPos_ = Model::GetBoneAnimPosition(pPlayerOrnament_->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayerOrnament_->GetModelHandle(), boneIndex_, partIndex_);

    if (transform_.rotate_.x >= 90.0f || transform_.rotate_.x <= -90.0f) {
        transform_.rotate_.y *= -1.0f;
        transform_.rotate_.z *= -1.0f;
    }
    transform_.rotate_.y += pPlayerOrnament_->GetRotate().y;

    Transform t = transform_;
    t.position_ = wandPos_;
    Model::SetTransform(hModel_, t);
    Model::Draw(hModel_);

}

void WeaponMainOrnament::Release()
{
}