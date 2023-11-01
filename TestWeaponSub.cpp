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
    pStateManager_->AddState(new TestWeaponSubWait(pStateManager_));
    pStateManager_->AddState(new TestWeaponSubCombo1(pStateManager_));
    pStateManager_->AddState(new TestWeaponSubCombo2(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    //モデルデータのロード
    hModel_ = Model::Load("Model/RedBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = (Player*)GetParent();

    transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
}

void TestWeaponSub::Update()
{
}

void TestWeaponSub::Draw()
{
    transform_.position_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), "hand.L");
    
    Transform t = transform_;
    CalcOffset(t);
    Model::SetTransform(hModel_, t);
    Model::Draw(hModel_);

}

void TestWeaponSub::Release()
{
}

void TestWeaponSub::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("Wait");
}

//--------------------------state-----------------------------------

TestWeaponSubWait::TestWeaponSubWait(StateManager* owner)
{
    owner_ = owner;
    pWeaponBase_ = static_cast<WeaponBase*>(owner_->GetGameObject());
}

void TestWeaponSubWait::Update()
{
    if (!pWeaponBase_->IsAtkEnd()) owner_->ChangeState("Combo1");
}

//---------------------------------------------------------------

TestWeaponSubCombo1::TestWeaponSubCombo1(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pWeaponBase_ = static_cast<WeaponBase*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
}

void TestWeaponSubCombo1::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdSubAtk()) next_ = true;

    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo2");
        else {
            pWeaponBase_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
    }
}

void TestWeaponSubCombo1::OnEnter()
{
    time_ = 20;
    next_ = false;
    pWeaponBase_->SetScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
}

void TestWeaponSubCombo1::OnExit()
{
    pWeaponBase_->SetScale(XMFLOAT3(0.2f, 0.2f, 0.2f));
}

//---------------------------------------------------------------

TestWeaponSubCombo2::TestWeaponSubCombo2(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pWeaponBase_ = static_cast<WeaponBase*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
}

void TestWeaponSubCombo2::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdSubAtk()) next_ = true;

    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else {
            pWeaponBase_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
    }
}

void TestWeaponSubCombo2::OnEnter()
{
    time_ = 20;
    next_ = false;
    pWeaponBase_->SetScale(XMFLOAT3(0.4f, 0.4f, 0.4f));
}

void TestWeaponSubCombo2::OnExit()
{
    pWeaponBase_->SetScale(XMFLOAT3(0.2f, 0.2f, 0.2f));
}
