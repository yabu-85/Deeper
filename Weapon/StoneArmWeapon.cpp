#include "StoneArmWeapon.h"
#include "../GameManager.h"
#include "../State/StateManager.h"
#include "../Engine/Model.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "../Enemy/EnemyBase.h"
#include "../Engine/SphereCollider.h"
#include "../VFXManager.h"

namespace {
    static const int COMBO_TIME1 = 100;
    static const float MOVE_SPEED = 0.03f;

}

StoneArmWeapon::StoneArmWeapon(GameObject* parent)
    : WeaponBase(parent, "StoneArmWeapon"), pPlayer_(nullptr), atkBoneIndex_(-1), atkPartIndex_(-1), pHandCollider_(nullptr),
    atkPosition_{0,0,0}
{
    transform_.pParent_ = nullptr;
    
}

StoneArmWeapon::~StoneArmWeapon()
{
}

void StoneArmWeapon::Initialize()
{
    hModel_ = Model::Load("Model/StoneArm.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = static_cast<Player*>(GetParent());
    durance_ = 10;
    float size = 1.0f;
    transform_.scale_ = { size, size, size };
    
    Model::GetBoneIndex(pPlayer_->GetModelHandle(), "forearm.R", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);
    Model::GetBoneIndex(hModel_, "Attack_Bone", &atkBoneIndex_, &atkPartIndex_);
    assert(atkBoneIndex_ >= 0);

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new StoneArmWeaponCombo1(pStateManager_));
    pStateManager_->ChangeState("");
    pStateManager_->Initialize();

    pHandCollider_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
    pHandCollider_->SetValid(false);
    AddAttackCollider(pHandCollider_);

}

void StoneArmWeapon::Update()
{
}

void StoneArmWeapon::Draw()
{
    if (!IsVisibled()) return;

    transform_.position_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);

    if (transform_.rotate_.x >= 90.0f || transform_.rotate_.x <= -90.0f) {
        transform_.rotate_.y *= -1.0f;
    }
    transform_.rotate_.y += pPlayer_->GetRotate().y;

    //‘½•ª«—ˆ’¼‚·C³‰ÓŠ
    atkPosition_ = Model::GetBoneAnimPosition(hModel_, atkBoneIndex_, atkPartIndex_);
    XMFLOAT3 p = pPlayer_->GetPosition();
    atkPosition_ = { atkPosition_.x - transform_.position_.x - p.x, 
        atkPosition_.y - transform_.position_.y - p.y, atkPosition_.z - transform_.position_.z - p.z };
    
    pHandCollider_->SetCenter(atkPosition_);
    pHandCollider_->SetValid(true);
    CollisionDraw();

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void StoneArmWeapon::Release()
{
}

void StoneArmWeapon::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("");
}

void StoneArmWeapon::ChangeAttackState()
{
    pStateManager_->ChangeState("Combo1");
    atkEnd_ = false;
}

void StoneArmWeapon::OnAttackCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        EnemyBase* e = static_cast<EnemyBase*>(pTarget);
        e->ApplyDamage(5);
        XMFLOAT3 pos = { -atkPosition_.x, -atkPosition_.y, -atkPosition_.z };
        VFXManager::CreatVfxExplode1(pos);
    }
}

//--------------------------state-----------------------------------

StoneArmWeaponCombo1::StoneArmWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void StoneArmWeaponCombo1::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (p->GetCommand()->CmdAtk()) next_ = true;

    time_++;
    if (time_ >= COMBO_TIME1) {
        StoneArmWeapon* s = static_cast<StoneArmWeapon*>(owner_->GetGameObject());
        s->ResetState();
    }
}

void StoneArmWeaponCombo1::OnEnter()
{
    time_ = 0;
    next_ = false;
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    Model::SetAnimFrame(p->GetModelHandle(), 700, 800, 1.0f);

}

void StoneArmWeaponCombo1::OnExit()
{
    StoneArmWeapon* s = static_cast<StoneArmWeapon*>(owner_->GetGameObject());
    s->Endurance();
    if (s->IsBlockend()) {
        s->SetAtkEnd(true);
    }
}