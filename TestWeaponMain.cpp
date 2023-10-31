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
    GameManager* pGameManager = (GameManager*)FindObject("GameManager");
    pDamageCtrl_ = pGameManager->GetDamageCtrl();

    transform_.scale_ = XMFLOAT3(0.2f, 1.0f, 0.2f);
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

void TestWeaponMain::CalcDamage(float range)
{
    XMFLOAT3 tar;
    tar.x = (float)sin(XMConvertToRadians(transform_.rotate_.y + offsetTrans_.rotate_.y));
    tar.y = (float)tan(XMConvertToRadians(transform_.rotate_.x + offsetTrans_.rotate_.x - 90.0));
    tar.z = (float)cos(XMConvertToRadians(transform_.rotate_.y + offsetTrans_.rotate_.y));

    XMFLOAT3 vec = tar;
    XMVECTOR vVec = XMLoadFloat3(&vec);
    vVec = XMVector3Normalize(vVec);
    XMStoreFloat3(&vec, vVec);
    pDamageCtrl_->CalcSword(transform_.position_, vec, range);

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
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 40;
}

void TestWeaponCombo1::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if(next_ == true) owner_->ChangeState("Combo2");
        else {
            pTestWeaponMain_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }

    float fff = (float)time_ / (float)comboTime_;
    pTestWeaponMain_->SetScale(XMFLOAT3(0.3f * fff, 1.0, 0.3 * fff));
    float rotateY = -30.0f + 60.0f * fff;
    pTestWeaponMain_->SetOffsetRotate(XMFLOAT3(90.0f, rotateY, 0.0f));
    pTestWeaponMain_->CalcDamage();

}

void TestWeaponCombo1::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo1::OnExit()
{
    pTestWeaponMain_->SetScale(XMFLOAT3(0.2f, 1.0f, 0.2f));
    pTestWeaponMain_->SetOffsetRotate(XMFLOAT3(90.0f, 0.0f, 0.0f));

}

//---------------------------------------------

TestWeaponCombo2::TestWeaponCombo2(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 25;
}

void TestWeaponCombo2::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo3");
        else {
            pTestWeaponMain_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }

    float fff = (float)time_ / (float)comboTime_;
    pTestWeaponMain_->SetScale(XMFLOAT3(0.5f * fff, 0.5f * fff, 0.5f * fff));
}

void TestWeaponCombo2::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo2::OnExit()
{
    pTestWeaponMain_->SetScale(XMFLOAT3(0.2f, 1.0f, 0.2f));
}

//---------------------------------------------

TestWeaponCombo3::TestWeaponCombo3(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 50;
}

void TestWeaponCombo3::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else {
            pTestWeaponMain_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }
    
    float fff = (float)time_ / (float)comboTime_;
    pTestWeaponMain_->SetScale(XMFLOAT3(0.7f * fff, 1.0f, 0.7f * fff));
    float rotateY = 45.0f - 90.0f * fff;
    pTestWeaponMain_->SetOffsetRotate(XMFLOAT3(90.0f, rotateY, 0.0f));
    pTestWeaponMain_->CalcDamage(20.0f);

}

void TestWeaponCombo3::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo3::OnExit()
{
    pTestWeaponMain_->SetScale(XMFLOAT3(0.2f, 1.0f, 0.2f));
    pTestWeaponMain_->SetOffsetRotate(XMFLOAT3(90.0f, 0.0f, 0.0f));
}
