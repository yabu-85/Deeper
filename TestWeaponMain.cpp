#include "TestWeaponMain.h"
#include "StateManager.h"
#include "Engine/Model.h"
#include "DamageCtrl.h"
#include "GameManager.h"
#include "Player.h"
#include "PlayerCommand.h"
#include "Engine/VFX.h"
#include "Engine/LineCollider.h"

TestWeaponMain::TestWeaponMain(GameObject* parent)
	:WeaponBase(parent), damage_(0)
{
	objectName_ = "TestWeaponMain";
}

TestWeaponMain::~TestWeaponMain()
{
    Release();
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

    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Model/BlueBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = (Player*)GetParent();
    GameManager* pGameManager = (GameManager*)FindObject("GameManager");
    pDamageCtrl_ = pGameManager->GetDamageCtrl();
    
    line_ = new LineCollider(XMFLOAT3(), XMFLOAT3(), 10.0f);
    AddCollider(line_);

    damage_ = 1;
}

void TestWeaponMain::Update()
{
}

void TestWeaponMain::Draw()
{
    wandPos_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), "Sword");
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), "Sword");
    transform_.rotate_.y += pPlayer_->GetUpRotate().y;

    Transform t = transform_;
    t.position_ = wandPos_;

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
    tar.y = -(float)tan(XMConvertToRadians(transform_.rotate_.x + offsetTrans_.rotate_.x));
    tar.z = (float)cos(XMConvertToRadians(transform_.rotate_.y + offsetTrans_.rotate_.y));

    XMFLOAT3 vec = tar;
    XMVECTOR vVec = XMLoadFloat3(&vec);
    vVec = XMVector3Normalize(vVec);
    XMStoreFloat3(&vec, vVec);
    
    line_->SetVec(vec);
    line_->SetSize(XMFLOAT3(range, range, range));
    pDamageCtrl_->CalcSword(line_, damage_);

    EmitterData  data;
    data.position = transform_.position_;
    data.position.x += wandPos_.x;
    data.position.y += wandPos_.y;
    data.position.z += wandPos_.z;
    data.delay = 0;
    data.number = 1;
    data.lifeTime = 5;
    data.positionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data.direction = vec;
    data.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data.speed = data.lifeTime * range / 40;
    data.speedRnd = 0.0f;
    data.accel = 1.0f;
    data.size = XMFLOAT2(0.4f, 0.4f);
    data.sizeRnd = XMFLOAT2(0.4f, 0.4f);
    data.scale = XMFLOAT2(1.0f, 1.0f);
    data.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
    data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    data.gravity = 0.0f;
    VFX::Start(data);

}

//--------------------state---------------------------------------------------

TestWeaponWait::TestWeaponWait(StateManager* owner)
{
    owner_ = owner;
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
}

void TestWeaponWait::Update()
{
    if (!pTestWeaponMain_->IsAtkEnd()) owner_->ChangeState("Combo1");
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
    if (pTestWeaponMain_->IsAtkEnd()) return;

    pPlayer_->CalcNoMove();
    pPlayer_->Move();
    pTestWeaponMain_->CalcDamage(3.0f);

    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if(next_ == true) owner_->ChangeState("Combo2");
        else {
            owner_->ChangeState("Wait");
            pTestWeaponMain_->SetAtkEnd(true);
        }
        return;
    }
}

void TestWeaponCombo1::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo1::OnExit()
{
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
    pPlayer_->CalcMove();
    pPlayer_->Move();
    pPlayer_->Rotate();
    pTestWeaponMain_->CalcDamage(5.0f);

    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo3");
        else {
            owner_->ChangeState("Wait");
            pTestWeaponMain_->SetAtkEnd(true);
        }
        return;
    }
}

void TestWeaponCombo2::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo2::OnExit()
{
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
    pPlayer_->CalcNoMove();
    pPlayer_->Move();
    pTestWeaponMain_->CalcDamage(8.0f);

    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else {
            owner_->ChangeState("Wait");
            pTestWeaponMain_->SetAtkEnd(true);
        }
        return;
    }
}

void TestWeaponCombo3::OnEnter()
{
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo3::OnExit()
{
}