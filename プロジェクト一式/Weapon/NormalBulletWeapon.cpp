#include "NormalBulletWeapon.h"
#include "NormalBullet.h"
#include "../State/StateManager.h"
#include "../Engine/Model.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/EnemyBase.h"

namespace {
    static const int COMBO_TIME1 = 20;
    static const int COMBO_TIME2 = 20;

}

NormalBulletWeapon::NormalBulletWeapon(GameObject* parent)
    : WeaponBase(parent, "NormalBulletWeapon"), pPlayer_(nullptr)
{
    transform_.pParent_ = nullptr;
    Invisible();
}

NormalBulletWeapon::~NormalBulletWeapon()
{
}

void NormalBulletWeapon::Initialize()
{
    hModel_ = Model::Load("Model/RedBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = static_cast<Player*>(GetParent());
    transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
    durance_ = 5;
    
    Model::GetBoneIndex(pPlayer_->GetModelHandle(), "Weapon", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new NormalBulletWeaponCombo1(pStateManager_));
    pStateManager_->ChangeState("");
    pStateManager_->Initialize();

}

void NormalBulletWeapon::Update()
{
}

void NormalBulletWeapon::Draw()
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

void NormalBulletWeapon::Release()
{
}

void NormalBulletWeapon::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("");
}

void NormalBulletWeapon::ChangeAttackState()
{
    pStateManager_->ChangeState("Combo1");
    atkEnd_ = false;
}

void NormalBulletWeapon::ShotBullet()
{
    Aim* pAim = pPlayer_->GetAim();
    XMFLOAT3 tar;
    if (pAim->IsTarget()) {
        tar = pAim->GetTargetPos();
        float aimTarPos = pAim->GetTargetEnemy()->GetAimTargetPos();
        tar = XMFLOAT3(tar.x, tar.y + aimTarPos, tar.z);
    }
    else {
        XMFLOAT3 pos = transform_.position_;
        XMFLOAT3 vec = pAim->GetAimDirection();
        tar = XMFLOAT3(pos.x + vec.x * 10.0f, pos.y + vec.y * 10.0f, pos.z + vec.z * 10.0f);
    }
    NormalBullet* b = Instantiate<NormalBullet>(pPlayer_->GetParent());
    b->Shot(transform_.position_, tar);

}

//--------------------------state-----------------------------------

NormalBulletWeaponCombo1::NormalBulletWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0)
{
}

void NormalBulletWeaponCombo1::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    p->CalcMove();
    p->Move();

    time_++;
    if (time_ >= COMBO_TIME1) {
        NormalBulletWeapon* s = static_cast<NormalBulletWeapon*>(owner_->GetGameObject());
        s->ResetState();
    }
}

void NormalBulletWeaponCombo1::OnEnter()
{
    time_ = 0;
    NormalBulletWeapon* s = static_cast<NormalBulletWeapon*>(owner_->GetGameObject());
    s->SetScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
    s->ShotBullet();

}

void NormalBulletWeaponCombo1::OnExit()
{
    NormalBulletWeapon* s = static_cast<NormalBulletWeapon*>(owner_->GetGameObject());
    s->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
    s->Endurance();
    if (s->IsBlockend()) {
        s->SetAtkEnd(true);
    }
}