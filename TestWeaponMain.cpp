#include "TestWeaponMain.h"
#include "StateManager.h"
#include "Engine/Model.h"
#include "DamageManager.h"
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

    //モデルデータのロード
    hModel_ = Model::Load("Model/BlueBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = (Player*)GetParent();
    pDamageManager_ = GameManager::GetDamageManager();
    
    line_ = new LineCollider(XMFLOAT3(), XMFLOAT3(), 10.0f);
    AddAttackCollider(line_);

    damage_ = 1;
}

void TestWeaponMain::Update()
{
}

void TestWeaponMain::Draw()
{
    wandPos_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), "Sword");
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), "Sword");
    
    if (transform_.rotate_.x >= 90.0f || transform_.rotate_.x <= -90.0f) {
        transform_.rotate_.y *= -1.0f;
        transform_.rotate_.z *= -1.0f;
    }
    transform_.rotate_.y += pPlayer_->GetRotate().y;

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
    XMFLOAT3 tar = XMFLOAT3(transform_.rotate_.x + offsetTrans_.rotate_.x, transform_.rotate_.y + offsetTrans_.rotate_.y, 0.0f);
    XMFLOAT3 target;
    target.x = (float)sin(XMConvertToRadians(tar.y));
    target.y = -(float)tan(XMConvertToRadians(tar.x));
    target.z = (float)cos(XMConvertToRadians(tar.y));

    if (tar.x >= 90.0f || tar.x <= -90.0f) {
        target.x *= -1.0f;
        target.y *= -1.0f;
        target.z *= -1.0f;
    }

    XMFLOAT3 vec = target;
    XMVECTOR vVec = XMLoadFloat3(&vec);
    vVec = XMVector3Normalize(vVec);
    XMStoreFloat3(&vec, vVec);
    
    line_->SetVec(vec);
    line_->SetSize(100.0f);
    pDamageManager_->CalcEnemy(line_, damage_);

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
    data.size = XMFLOAT2(0.1f, 0.1f);
    data.sizeRnd = XMFLOAT2(0.4f, 0.4f);
    data.scale = XMFLOAT2(1.0f, 1.0f);
    data.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
    data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    data.gravity = 0.0f;
    VFX::Start(data);

}

//--------------------state---------------------------------------------------

TestWeaponWait::TestWeaponWait(StateManager* owner) : StateBase(owner)
{
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
}

void TestWeaponWait::Update()
{
    if (!pTestWeaponMain_->IsAtkEnd()) owner_->ChangeState("Combo1");
}

//---------------------------------------------

TestWeaponCombo1::TestWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 40;
}

void TestWeaponCombo1::Update()
{
    if (pTestWeaponMain_->IsAtkEnd()) return;

    pPlayer_->CalcNoMove();
    pPlayer_->Move();
    pTestWeaponMain_->CalcDamage(7.0f);

    if (time_ > (comboTime_ - 10)) {
        pPlayer_->CalcRotate();
        pPlayer_->Rotate(0.15f);
    }
    
    float speed = (float)time_ / (float)comboTime_;
    pPlayer_->FrontMove(speed);

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
    Model::SetAnimFrame(pPlayer_->GetModelHandle(), 0, 40, 1.0f);
}

void TestWeaponCombo1::OnExit()
{
    Model::SetAnimFrame(pPlayer_->GetModelHandle(), 0, 0, 1.0f);
}

//---------------------------------------------

TestWeaponCombo2::TestWeaponCombo2(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 40;
}

void TestWeaponCombo2::Update()
{
    pPlayer_->CalcNoMove();
    pPlayer_->Move();
    pTestWeaponMain_->CalcDamage(7.0f);

    if (time_ > (comboTime_ - 10)) {
        pPlayer_->CalcRotate();
        pPlayer_->Rotate(0.15f);
    }
    float speed = (float)time_ / (float)comboTime_;
    pPlayer_->FrontMove(speed * 0.5f);

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
    Model::SetAnimFrame(pPlayer_->GetModelHandle(), 40, 80, 1.0f);
}

void TestWeaponCombo2::OnExit()
{
    Model::SetAnimFrame(pPlayer_->GetModelHandle(), 0, 0, 1.0f);
}

//---------------------------------------------

TestWeaponCombo3::TestWeaponCombo3(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 70;
}

void TestWeaponCombo3::Update()
{
    pPlayer_->CalcNoMove();
    pPlayer_->Move();
    pTestWeaponMain_->CalcDamage(7.0f);

    if (time_ > (comboTime_ - 10)) {
        pPlayer_->CalcRotate();
        pPlayer_->Rotate(0.15f);
    }
    float speed = (float)time_ / (float)comboTime_;
    pPlayer_->FrontMove(speed);

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
    Model::SetAnimFrame(pPlayer_->GetModelHandle(), 80, 150, 1.0f);
}

void TestWeaponCombo3::OnExit()
{
    Model::SetAnimFrame(pPlayer_->GetModelHandle(), 0, 0, 1.0f);
}