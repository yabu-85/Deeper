#include "TestWeaponSub.h"
#include "StateManager.h"
#include "Engine/Model.h"
#include "DamageManager.h"
#include "GameManager.h"
#include "Player.h"
#include "PlayerCommand.h"
#include "TestBullet.h"
#include "Aim.h"
#include "EnemyBase.h"

TestWeaponSub::TestWeaponSub(GameObject* parent)
    :WeaponBase(parent)
{
    std::string str;
    str = std::to_string(rand() % 1000);
    objectName_ = "TestWeaponSub" + str;
}

TestWeaponSub::~TestWeaponSub()
{
}

void TestWeaponSub::Initialize()
{
    hModel_ = Model::Load("Model/RedBox.fbx");
    assert(hModel_ >= 0);

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new TestWeaponSubWait(pStateManager_));
    pStateManager_->AddState(new TestWeaponSubCombo1(pStateManager_));
    pStateManager_->AddState(new TestWeaponSubCombo2(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    offsetTrans_.position_.y += (float)(rand() % 10) * 0.1f;
    transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
    endurance_ = 50;

    pPlayer_ = (Player*)GetParent();
}

void TestWeaponSub::Update()
{
}

void TestWeaponSub::Draw()
{
    if (!IsVisibled()) return;

    transform_.position_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), "Sword");
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), "Sword");
    if (transform_.rotate_.x >= 90.0f || transform_.rotate_.x <= -90.0f) {
        transform_.rotate_.y *= -1.0f;
        transform_.rotate_.z *= -1.0f;
    }
    transform_.rotate_.y += pPlayer_->GetRotate().y;

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

void TestWeaponSub::ShotBullet()
{
    Aim* pAim = pPlayer_->GetAim();
    XMFLOAT3 tar;
    if (pAim->IsTarget()) {
        tar = pAim->GetTargetPos();
        float aimTarPos =  pAim->GetTargetEnemy()->GetAimTargetPos();
        tar = XMFLOAT3(tar.x, tar.y + aimTarPos, tar.z);
    }
    else {
        XMFLOAT3 pos = transform_.position_;
        XMFLOAT3 vec = pAim->GetAimDirection();
        tar = XMFLOAT3(pos.x + vec.x, pos.y + vec.y, pos.z + vec.z);
    }
    TestBullet* b = Instantiate<TestBullet>(pPlayer_->GetParent());
    b->Shot(transform_.position_, tar);
}

//--------------------------state-----------------------------------

TestWeaponSubWait::TestWeaponSubWait(StateManager* owner) : StateBase(owner)
{
    pTestWeaponSub_ = static_cast<TestWeaponSub*>(owner_->GetGameObject());
}

void TestWeaponSubWait::Update()
{
    if (!pTestWeaponSub_->IsAtkEnd()) owner_->ChangeState("Combo1");
}

//---------------------------------------------------------------

TestWeaponSubCombo1::TestWeaponSubCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
    pTestWeaponSub_ = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
}

void TestWeaponSubCombo1::Update()
{
    pPlayer_->CalcMove();
    pPlayer_->Move();

    time_--;
    if (pPlayer_->GetCommand()->CmdSubAtk()) next_ = true;

    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo2");
        else {
            owner_->ChangeState("Wait");
            pTestWeaponSub_->SetAtkEnd(true);
        }
        return;
    }

}

void TestWeaponSubCombo1::OnEnter()
{
    time_ = 20;
    next_ = false;
    if (pTestWeaponSub_ == nullptr) owner_->ChangeState("Wait");
    pTestWeaponSub_->SetScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
    
    pTestWeaponSub_->ShotBullet();

}

void TestWeaponSubCombo1::OnExit()
{
    pTestWeaponSub_->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
    pTestWeaponSub_->Endurance();
    if (pTestWeaponSub_->IsBlockend()) {
        pTestWeaponSub_->SetAtkEnd(true);
    }
}

//---------------------------------------------------------------

TestWeaponSubCombo2::TestWeaponSubCombo2(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
    pTestWeaponSub_ = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
}

void TestWeaponSubCombo2::Update()
{
    pPlayer_->CalcMove();
    pPlayer_->Move();

    time_--;
    if (pPlayer_->GetCommand()->CmdSubAtk()) next_ = true;

    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else {
            owner_->ChangeState("Wait");
            pTestWeaponSub_->SetAtkEnd(true);
        }
        return;
    }

    pTestWeaponSub_->ShotBullet();
}

void TestWeaponSubCombo2::OnEnter()
{
    time_ = 20;
    next_ = false;
    if (pTestWeaponSub_ == nullptr) owner_->ChangeState("Wait");
    pTestWeaponSub_->SetScale(XMFLOAT3(0.4f, 0.4f, 0.4f));
}

void TestWeaponSubCombo2::OnExit()
{
    pTestWeaponSub_->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
    pTestWeaponSub_->Endurance();
    if (pTestWeaponSub_->IsBlockend()) {
        pTestWeaponSub_->SetAtkEnd(true);
    }
}
