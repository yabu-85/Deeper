#include "TestWeaponSub.h"
#include "TestBullet.h"
#include "ThrowBullet.h"

#include "../State/StateManager.h"
#include "../Engine/Model.h"
#include "../GameManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "../Player/Aim.h"
#include "../Enemy/EnemyBase.h"

namespace {
    static const int COMBO_TIME1 = 20;
    static const int COMBO_TIME2 = 20;

}

TestWeaponSub::TestWeaponSub(GameObject* parent)
    : WeaponBase(parent, "TestWeaponSub"), pPlayer_(nullptr)
{
    transform_.pParent_ = nullptr;

}

TestWeaponSub::~TestWeaponSub()
{
}

void TestWeaponSub::Initialize()
{
    hModel_ = Model::Load("Model/RedBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = static_cast<Player*>(GetParent());
    transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
    durance_ = 5;
    
    Model::GetBoneIndex(pPlayer_->GetModelHandle(), "Weapon", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new TestWeaponSubCombo1(pStateManager_));
    pStateManager_->AddState(new TestWeaponSubCombo2(pStateManager_));
    pStateManager_->ChangeState("");
    pStateManager_->Initialize();

}

void TestWeaponSub::Update()
{
}

void TestWeaponSub::Draw()
{
    if (!IsVisibled()) return;

    transform_.position_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);

    if (transform_.rotate_.x >= 90.0f || transform_.rotate_.x <= -90.0f) {
        transform_.rotate_.y *= -1.0f;
        transform_.rotate_.z *= -1.0f;
    }
    transform_.rotate_.y += pPlayer_->GetRotate().y;

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void TestWeaponSub::Release()
{
}

void TestWeaponSub::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("");
}

void TestWeaponSub::ChangeAttackState()
{
    pStateManager_->ChangeState("Combo1");
    atkEnd_ = false;
}

void TestWeaponSub::ShotBullet()
{
    Aim* pAim = pPlayer_->GetAim();
    XMFLOAT3 tar;
    if (pAim->IsTarget()) {
        tar = pAim->GetTargetPos();
        float aimTarPos = pAim->GetTargetEnemy()->GetAimTargetPos();
    //    tar = XMFLOAT3(tar.x, tar.y + aimTarPos, tar.z);
        tar = XMFLOAT3(tar.x, tar.y, tar.z);
    }
    else {
        XMFLOAT3 pos = transform_.position_;
        XMFLOAT3 vec = pAim->GetAimDirection();
        tar = XMFLOAT3(pos.x + vec.x * 10.0f, pos.y + vec.y * 10.0f, pos.z + vec.z * 10.0f);
    }
    ThrowBullet* b = Instantiate<ThrowBullet>(pPlayer_->GetParent());
    b->Shot(transform_.position_, tar);

}

//--------------------------state-----------------------------------

TestWeaponSubCombo1::TestWeaponSubCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void TestWeaponSubCombo1::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    p->CalcMove();
    p->Move();
    if (p->GetCommand()->CmdSubAtk()) next_ = true;

    time_++;
    if (time_ >= COMBO_TIME1) {
        TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
        if (next_ == true) owner_->ChangeState("Combo2");
        else s->ResetState();
    }
}

void TestWeaponSubCombo1::OnEnter()
{
    time_ = 0;
    next_ = false;
    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    s->SetScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
    s->ShotBullet();

}

void TestWeaponSubCombo1::OnExit()
{
    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    s->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
    s->Endurance();
    if (s->IsBlockend()) {
        s->SetAtkEnd(true);
    }
}

//---------------------------------------------------------------

TestWeaponSubCombo2::TestWeaponSubCombo2(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void TestWeaponSubCombo2::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    p->CalcMove();
    p->Move();
    if (p->GetCommand()->CmdSubAtk()) next_ = true;
    
    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    s->ShotBullet();

    time_++;
    if (time_ >= COMBO_TIME2) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else s->ResetState();
    }
}

void TestWeaponSubCombo2::OnEnter()
{
    time_ = 0;
    next_ = false;
    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    s->SetScale(XMFLOAT3(0.4f, 0.4f, 0.4f));
}

void TestWeaponSubCombo2::OnExit()
{
    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    s->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
    s->Endurance();
    if (s->IsBlockend()) {
        s->SetAtkEnd(true);
    }
}
