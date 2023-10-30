#include "TestWeaponSub.h"
#include "StateManager.h"
#include "Engine/Model.h"
#include "DamageCtrl.h"
#include "GameManager.h"
#include "Player.h"
#include "PlayerCommand.h"

TestWeaponSub::TestWeaponSub(GameObject* parent)
    :WeaponBase(parent)
{
    objectName_ = "TestWeaponSub";
}

TestWeaponSub::~TestWeaponSub()
{
}

void TestWeaponSub::Initialize()
{
    pStateManager_ = new StateManager(this);
//    pStateManager_->AddState(new TestWeaponWait(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    //モデルデータのロード
    hModel_ = Model::Load("Model/RedBox.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
}

void TestWeaponSub::Update()
{
    transform_.position_ = GetParent()->GetPosition();
    transform_.position_.x += offsetPosition_.x;
    transform_.position_.y += offsetPosition_.y;
    transform_.position_.z += offsetPosition_.z;

    transform_.rotate_.x += offsetRotation_.x;
    transform_.rotate_.y += offsetRotation_.y;
    transform_.rotate_.z += offsetRotation_.z;
}

void TestWeaponSub::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void TestWeaponSub::Release()
{
}

void TestWeaponSub::ResetState()
{
}
