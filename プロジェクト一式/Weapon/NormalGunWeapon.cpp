#include "NormalGunWeapon.h"
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

NormalGunWeapon::NormalGunWeapon(GameObject* parent)
    : WeaponBase(parent, "NormalGunWeapon"), pPlayer_(nullptr)
{
    transform_.pParent_ = nullptr;
    Invisible();
}

NormalGunWeapon::~NormalGunWeapon()
{
}

void NormalGunWeapon::Initialize()
{
    hModel_ = Model::Load("Model/RedBox.fbx");
    assert(hModel_ >= 0);

    type_ = ENEMY_TYPE::ENEMY_THROW;
    pPlayer_ = static_cast<Player*>(GetParent());
    transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
    durance_ = 6;
    
    Model::GetBoneIndex(pPlayer_->GetModelHandle(), "Weapon", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new NormalGunWeaponCombo1(pStateManager_));
    pStateManager_->ChangeState("");
    pStateManager_->Initialize();

}

void NormalGunWeapon::Update()
{
}

void NormalGunWeapon::Draw()
{
    if (!IsVisibled()) return;

    transform_.position_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_.y += pPlayer_->GetRotate().y;

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void NormalGunWeapon::Release()
{
}

void NormalGunWeapon::ResetState()
{
    isAtkEnd_ = true;
    pStateManager_->ChangeState("");
}

void NormalGunWeapon::ChangeAttackState()
{
    pStateManager_->ChangeState("Combo1");
    isAtkEnd_ = false;
}

void NormalGunWeapon::ShotBullet()
{
    transform_.position_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
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
        tar = XMFLOAT3(pos.x + vec.x, pos.y + vec.y, pos.z + vec.z);
    }
    NormalBullet* b = Instantiate<NormalBullet>(pPlayer_->GetParent());
    b->SetShotParent(pPlayer_);
    b->Shot(transform_.position_, tar);

}

//--------------------------state-----------------------------------

NormalGunWeaponCombo1::NormalGunWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0)
{
}

void NormalGunWeaponCombo1::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    p->CalcMove();
    p->Move();

    time_++;
    if (time_ >= COMBO_TIME1) {
        NormalGunWeapon* s = static_cast<NormalGunWeapon*>(owner_->GetGameObject());
        s->ResetState();
    }
}

void NormalGunWeaponCombo1::OnEnter()
{
    time_ = 0;
    NormalGunWeapon* s = static_cast<NormalGunWeapon*>(owner_->GetGameObject());
    s->SetScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
    s->ShotBullet();

}

void NormalGunWeaponCombo1::OnExit()
{
    NormalGunWeapon* s = static_cast<NormalGunWeapon*>(owner_->GetGameObject());
    s->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
    s->Endurance();
    if (s->IsBlockend()) {
        s->SetAtkEnd(true);
    }
}