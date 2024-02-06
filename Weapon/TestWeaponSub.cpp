#include "TestWeaponSub.h"
#include "../State/StateManager.h"
#include "../Engine/Model.h"
#include "../GameManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "TestBullet.h"
#include "../Player/Aim.h"
#include "../Enemy/EnemyBase.h"

TestWeaponSub::TestWeaponSub(GameObject* parent)
    : WeaponBase(parent, "TestWeaponSub")
{
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
    durance_ = 50;

    Player* p = static_cast<Player*>(GetParent());
    Model::GetBoneIndex(p->GetModelHandle(), "Weapon", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);

}

void TestWeaponSub::Update()
{
}

void TestWeaponSub::Draw()
{
    if (!IsVisibled()) return;

    Player* p = static_cast<Player*>(GetParent());
    transform_.position_ = Model::GetBoneAnimPosition(p->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_ = Model::GetBoneAnimRotate(p->GetModelHandle(), boneIndex_, partIndex_);

    if (transform_.rotate_.x >= 90.0f || transform_.rotate_.x <= -90.0f) {
        transform_.rotate_.y *= -1.0f;
        transform_.rotate_.z *= -1.0f;
    }
    transform_.rotate_.y += p->GetRotate().y;

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
    Player* p = static_cast<Player*>(GetParent());
    Aim* pAim = p->GetAim();
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
    TestBullet* b = Instantiate<TestBullet>(p->GetParent());
    b->Shot(transform_.position_, tar);
}

//--------------------------state-----------------------------------

TestWeaponSubWait::TestWeaponSubWait(StateManager* owner) : StateBase(owner)
{
}

void TestWeaponSubWait::Update()
{
    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    if (!s->IsAtkEnd()) owner_->ChangeState("Combo1");
}

//---------------------------------------------------------------

TestWeaponSubCombo1::TestWeaponSubCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void TestWeaponSubCombo1::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    p->CalcMove();
    p->Move();

    time_--;
    PlayerCommand* cmd = p->GetCommand();
    if (cmd && p->GetCommand()->CmdSubAtk()) next_ = true;

    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo2");
        else {
            owner_->ChangeState("Wait");
            s->SetAtkEnd(true);
        }
        return;
    }

}

void TestWeaponSubCombo1::OnEnter()
{
    time_ = 20;
    next_ = false;
    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    if (s == nullptr) owner_->ChangeState("Wait");
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

    time_--;
    if (p->GetCommand()->CmdSubAtk()) next_ = true;

    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else {
            owner_->ChangeState("Wait");
            s->SetAtkEnd(true);
        }
        return;
    }

    s->ShotBullet();
}

void TestWeaponSubCombo2::OnEnter()
{
    time_ = 20;
    next_ = false;
    TestWeaponSub* s = static_cast<TestWeaponSub*>(owner_->GetGameObject());
    if (s == nullptr) owner_->ChangeState("Wait");
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
