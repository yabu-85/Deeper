#include "StoneArmWeapon.h"
#include "../VFXManager.h"
#include "../InputManager.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../State/StateManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/EnemyBase.h"

namespace {
    static const int ATTACK_DAMAGE = 100;
    static const float MOVE_SPEED = 0.03f;

    XMVECTOR shakeDirection { 0.0f, 1.0f, 0.0f, 0.0f };
    CameraShakeInfo shakeInfo(7, 0.28f, 0.7f, 0.3f, 0.8f);

    static const int COMBO_TIME1 = 100;
    static const int ATTACK_FRAME1 = 52;   //”»’èƒtƒŒ[ƒ€
    static const int ROTATE_FRAME[2] = { 5, 15 };
}

StoneArmWeapon::StoneArmWeapon(GameObject* parent)
    : WeaponBase(parent, "StoneArmWeapon"), pPlayer_(nullptr), atkBoneIndex_(-1), atkPartIndex_(-1), pHandCollider_(nullptr),
    atkPosition_{0,0,0}
{
    transform_.pParent_ = nullptr;
    Invisible();
}

StoneArmWeapon::~StoneArmWeapon()
{
}

void StoneArmWeapon::Initialize()
{
    hModel_ = Model::Load("Model/StoneArm.fbx");
    assert(hModel_ >= 0);

    type_ = WeaponObjectManager::WEAPON_TYPE::WT_STONE;
    pPlayer_ = GameManager::GetPlayer();
    durance_ = 3;
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
    transform_.rotate_.y += pPlayer_->GetRotate().y;

    //‘½•ª«—ˆ’¼‚·C³‰ÓŠ
    atkPosition_ = Model::GetBoneAnimPosition(hModel_, atkBoneIndex_, atkPartIndex_);
    XMFLOAT3 p = pPlayer_->GetPosition();
    p = { atkPosition_.x - transform_.position_.x - p.x, atkPosition_.y - transform_.position_.y - p.y, atkPosition_.z - transform_.position_.z - p.z };
    pHandCollider_->SetCenter(p);
    CollisionDraw();

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void StoneArmWeapon::Release()
{
}

void StoneArmWeapon::ResetState()
{
    isAtkEnd_ = true;
    pStateManager_->ChangeState("");
}

void StoneArmWeapon::ChangeAttackState()
{
    pStateManager_->ChangeState("Combo1");
    isAtkEnd_ = false;
}

void StoneArmWeapon::OnAttackCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        EnemyBase* e = static_cast<EnemyBase*>(pTarget);
        DamageInfo damage(ATTACK_DAMAGE);
        KnockBackInfo knock(KNOCK_TYPE::MEDIUM, 30, 0.3f, pPlayer_->GetPosition());
        e->ApplyDamageDirectly(damage, knock);
    }
}

void StoneArmWeapon::Attack()
{
    VFXManager::CreatVfxSmoke(atkPosition_);

    GameManager::GetPlayer()->GetAim()->SetCameraShakeDirection(shakeDirection);
    GameManager::GetPlayer()->GetAim()->SetCameraShake(shakeInfo);
}

//--------------------------state-----------------------------------

StoneArmWeaponCombo1::StoneArmWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0)
{
}

void StoneArmWeaponCombo1::Update()
{
    //ˆÚ“®‚â‚ç‰ñ“]‚â‚ç
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (time_ >= ROTATE_FRAME[0] && time_ <= ROTATE_FRAME[1]) {
        float rRatio = (float)time_ / (float)ROTATE_FRAME[1];
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(rRatio);
        else if (InputManager::CmdWalk()) p->Rotate(rRatio);
    }

    //”»’è
    StoneArmWeapon* s = static_cast<StoneArmWeapon*>(owner_->GetGameObject());
    if (time_ == ATTACK_FRAME1) {
        s->GetSphereCollider()->SetValid(true);
        s->Attack();
    }
    else {
        s->GetSphereCollider()->SetValid(false);
    }

    //I—¹”»’è
    time_++;
    if (time_ >= COMBO_TIME1) {
        StoneArmWeapon* s = static_cast<StoneArmWeapon*>(owner_->GetGameObject());
        s->ResetState();
    }
}

void StoneArmWeaponCombo1::OnEnter()
{
    time_ = 0;
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    p->GetAnimationController()->SetNextAnime((int)PLAYER_ANIMATION::STONE_ATTACK, 0.2f);

}

void StoneArmWeaponCombo1::OnExit()
{
    StoneArmWeapon* s = static_cast<StoneArmWeapon*>(owner_->GetGameObject());
    s->Endurance();
    if (s->IsBlockend()) {
        s->SetAtkEnd(true);
    }
}
