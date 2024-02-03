#include "TestWeaponMain.h"
#include "../State/StateManager.h"
#include "../Engine/Model.h"
#include "../GameManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "../Engine/SegmentCollider.h"
#include "../VFXManager.h"
#include "../Player/Aim.h"

#include "../Engine/PolyLine.h"
#include "../Engine/Global.h"

namespace {
    float weaponSize = 5.0f;

}

TestWeaponMain::TestWeaponMain(GameObject* parent)
	: WeaponBase(parent, "TestWeaponMain"), pPlayer_(nullptr), seg_(nullptr), damage_(0), wandPos_(0,0,0), pPolyLine_(nullptr)
{
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
    
    seg_ = new SegmentCollider(XMFLOAT3(), XMVECTOR());
    seg_->SetValid(false);
    AddAttackCollider(seg_);

    damage_ = 20; 
    transform_.scale_.z = 2.0f;
    
    pPolyLine_ = new PolyLine;
    pPolyLine_->Load("tex.png");
    pPolyLine_->AddPosition(transform_.position_);

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

    pPolyLine_->Draw();
}

void TestWeaponMain::Release()
{
    SAFE_RELEASE(pPolyLine_);
    SAFE_DELETE_ARRAY(pPolyLine_);

}

void TestWeaponMain::OnAttackCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        EnemyBase* e = static_cast<EnemyBase*>(pTarget);
        e->ApplyDamage(damage_);
        VFXManager::CreatVfxExplode1(wandPos_);
        seg_->SetValid(false);
    }

}

void TestWeaponMain::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("Wait");
}

void TestWeaponMain::CalcDamage()
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
    vVec = XMVector3Normalize(vVec) * weaponSize;
    seg_->SetVector(vVec);
    seg_->SetValid(true);

    XMStoreFloat3(&vec, vVec * 0.5f);
    vec = XMFLOAT3(wandPos_.x + vec.x, wandPos_.y + vec.y, wandPos_.z + vec.z);
    pPolyLine_->AddPosition(vec);

}

//--------------------state---------------------------------------------------

TestWeaponWait::TestWeaponWait(StateManager* owner) : StateBase(owner)
{
}

void TestWeaponWait::Update()
{
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    if (!m->IsAtkEnd()) owner_->ChangeState("Combo1");
}

//---------------------------------------------

TestWeaponCombo1::TestWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 40;
}

void TestWeaponCombo1::Update()
{
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    if (m->IsAtkEnd()) return;

    pPlayer_->CalcNoMove();
    pPlayer_->Move();

    m->GetSegmentCollider()->SetValid(false);
    if(time_ > 14 && time_ < 24)
    m->CalcDamage();

    if (time_ < 5) {
        if (pPlayer_->GetAim()->IsTarget()) pPlayer_->AimTargetRotate(0.3f);
        else if(pPlayer_->GetCommand()->CmdWalk()) pPlayer_->Rotate(0.15f);
    }
    
    pPlayer_->FrontMove(0.2f);

    time_++;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ >= comboTime_) {
        if(next_ == true) owner_->ChangeState("Combo2");
        else {
            owner_->ChangeState("Wait");
            m->SetAtkEnd(true);
        }
        return;
    }
}

void TestWeaponCombo1::OnEnter()
{
    time_ = 0;
    next_ = false;
    Model::SetAnimFrame(pPlayer_->GetModelHandle(), 0, 40, 1.0f);
}

void TestWeaponCombo1::OnExit()
{
    Model::SetAnimFrame(pPlayer_->GetModelHandle(), 0, 0, 1.0f);
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}

//---------------------------------------------

TestWeaponCombo2::TestWeaponCombo2(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 40;
}

void TestWeaponCombo2::Update()
{
    pPlayer_->CalcNoMove();
    pPlayer_->Move();
    
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if (time_ > 10 && time_ < 40 - 15)
    m->CalcDamage();

    if (time_ > (comboTime_ - 10)) {
        if (pPlayer_->GetAim()->IsTarget()) pPlayer_->AimTargetRotate(0.3f);
        else if (pPlayer_->GetCommand()->CmdWalk()) pPlayer_->Rotate(0.15f);
    }

    float speed = (float)time_ / (float)comboTime_;
    pPlayer_->FrontMove(speed * 0.5f);

    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo3");
        else {
            owner_->ChangeState("Wait");
            m->SetAtkEnd(true);
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
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}

//---------------------------------------------

TestWeaponCombo3::TestWeaponCombo3(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 70;
}

void TestWeaponCombo3::Update()
{
    pPlayer_->CalcNoMove();
    pPlayer_->Move();
    
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if (time_ > 35 && time_ < 70 - 25)
    m->CalcDamage();

    if (time_ > (comboTime_ - 10)) {
        if (pPlayer_->GetAim()->IsTarget()) pPlayer_->AimTargetRotate(0.3f);
        else if (pPlayer_->GetCommand()->CmdWalk()) pPlayer_->Rotate(0.15f);
    }

    float speed = (float)time_ / (float)comboTime_;
    pPlayer_->FrontMove(speed);

    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else {
            owner_->ChangeState("Wait");
            m->SetAtkEnd(true);
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
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}