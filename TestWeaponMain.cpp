#include "TestWeaponMain.h"
#include "StateManager.h"
#include "Engine/Model.h"
#include "DamageCtrl.h"
#include "GameManager.h"
#include "Player.h"
#include "PlayerCommand.h"

TestWeaponMain::TestWeaponMain(GameObject* parent)
	:WeaponBase(parent)
{
	objectName_ = "TestWeaponMain";
}

TestWeaponMain::~TestWeaponMain()
{
}

void TestWeaponMain::Initialize()
{
    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new TestWeaponWait(pStateManager_));
    pStateManager_->AddState(new TestWeaponCombo1(pStateManager_));
    pStateManager_->AddState(new TestWeaponCombo2(pStateManager_));
    pStateManager_->AddState(new TestWeaponCombo3(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    //モデルデータのロード
    hModel_ = Model::Load("Model/BlueBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = (Player*)GetParent();

    transform_.scale_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
}

void TestWeaponMain::Update()
{

}

void TestWeaponMain::Draw()
{
    //Drawの直前にやらないとひとつ前のフレームの座標が返ってくる
    transform_.position_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), "hand.R");
    
    Transform t = transform_;
    CalcOffset(t);
    Model::SetTransform(hModel_, t);
    Model::Draw(hModel_);

}

void TestWeaponMain::Release()
{
}

void TestWeaponMain::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("Wait");
}

//--------------------state---------------------------------------------------

TestWeaponWait::TestWeaponWait(StateManager* owner)
{
    owner_ = owner;
    pWeaponBase_ = static_cast<WeaponBase*>(owner_->GetGameObject());
}

void TestWeaponWait::Update()
{
    if (!pWeaponBase_->IsAtkEnd()) owner_->ChangeState("Combo1");
}

//---------------------------------------------

TestWeaponCombo1::TestWeaponCombo1(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pWeaponBase_ = static_cast<WeaponBase*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    GameManager* pGameManager = (GameManager*)owner_->GetGameObject()->FindObject("GameManager");
    pDamageCtrl_ = pGameManager->GetDamageCtrl();
    comboTime_ = 20;
}

void TestWeaponCombo1::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if(next_ == true) owner_->ChangeState("Combo2");
        else {
            pWeaponBase_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }

    float fff = (float)time_ / (float)comboTime_;
    pWeaponBase_->SetScale(XMFLOAT3(0.3f * fff, 0.3f * fff, 0.3f * fff));
}

void TestWeaponCombo1::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
    pDamageCtrl_->ApplyDamage(DamageCtrl::RAND, 2);
}

void TestWeaponCombo1::OnExit()
{
    pWeaponBase_->SetScale(XMFLOAT3(0.2f, 0.2f, 0.2f));
}

//---------------------------------------------

TestWeaponCombo2::TestWeaponCombo2(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pWeaponBase_ = static_cast<WeaponBase*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    GameManager* pGameManager = (GameManager*)owner_->GetGameObject()->FindObject("GameManager");
    pDamageCtrl_ = pGameManager->GetDamageCtrl();
    comboTime_ = 25;
}

void TestWeaponCombo2::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo3");
        else {
            pWeaponBase_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }

    float fff = (float)time_ / (float)comboTime_;
    pWeaponBase_->SetScale(XMFLOAT3(0.5f * fff, 0.5f * fff, 0.5f * fff));

}

void TestWeaponCombo2::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
    pDamageCtrl_->ApplyDamage(DamageCtrl::RAND, 2);
}

void TestWeaponCombo2::OnExit()
{
    pWeaponBase_->SetScale(XMFLOAT3(0.2f, 0.2f, 0.2f));
}

//---------------------------------------------

TestWeaponCombo3::TestWeaponCombo3(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pWeaponBase_ = static_cast<WeaponBase*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    GameManager* pGameManager = (GameManager*)owner_->GetGameObject()->FindObject("GameManager");
    pDamageCtrl_ = pGameManager->GetDamageCtrl();
    comboTime_ = 30;
}

void TestWeaponCombo3::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else {
            pWeaponBase_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }
    
    float fff = (float)time_ / (float)comboTime_;
    pWeaponBase_->SetScale(XMFLOAT3(0.7f * fff, 0.7f * fff, 0.7f * fff));

}

void TestWeaponCombo3::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
    pDamageCtrl_->ApplyDamage(DamageCtrl::ALL, 2);
    
}

void TestWeaponCombo3::OnExit()
{
    pWeaponBase_->SetScale(XMFLOAT3(0.2f, 0.2f, 0.2f));
}
