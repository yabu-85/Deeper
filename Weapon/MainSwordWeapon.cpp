#include "MainSwordWeapon.h"
#include "../GameManager/GameManager.h"
#include "../VFXManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "../Player/Aim.h"
#include "../Engine/PolyLine.h"
#include "../Engine/Global.h"
#include "../Engine/SegmentCollider.h"
#include "../Engine/Model.h"
#include "../State/StateManager.h"
#include "../Enemy/EnemyManager.h"

namespace {
    static const float WEAPON_SIZE = 1.2f;
    static const float MOVE_SPEED = 0.3f;
    static const float ROTATE_RATIO = 0.2f;
    
    //回転フレーム
    static const int ROTATE_FRAME[2] = { 3, 10 };

    //攻撃アニメーションのフレーム
    static const int ANIM_FRAME1[2] = { 595, 650 };
    static const int ANIM_FRAME2[2] = { 650, 680 };
    static const int ANIM_FRAME3[2] = { 665, 680 };
    
    //10から～30フレームまでが攻撃フレーム
    static const int ANIM_ATTACK_FRAME1[2] = { 17, 33 };
    static const int ANIM_ATTACK_FRAME2[2] = { 15, 25 };
    static const int ANIM_ATTACK_FRAME3[2] = { 0, 25 };

    static const int COMBO_TIME1 = ANIM_FRAME1[1] - ANIM_FRAME1[0];
    static const int COMBO_TIME2 = ANIM_FRAME2[1] - ANIM_FRAME2[0];
    static const int COMBO_TIME3 = ANIM_FRAME3[1] - ANIM_FRAME3[0];

}

MainSwordWeapon::MainSwordWeapon(GameObject* parent)
	: WeaponBase(parent, "MainSwordWeapon"), pPlayer_(nullptr), seg_(nullptr), damage_(0), wandPos_(0,0,0), pPolyLine_(nullptr)
{
    transform_.pParent_ = nullptr;
}

MainSwordWeapon::~MainSwordWeapon()
{
}

void MainSwordWeapon::Initialize()
{
    hModel_ = Model::Load("Model/BlueBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = static_cast<Player*>(GetParent());
    Model::GetBoneIndex(pPlayer_->GetModelHandle(), "Weapon", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);

    damage_ = 20;

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new MainSwordWeaponCombo1(pStateManager_));
    pStateManager_->AddState(new MainSwordWeaponCombo2(pStateManager_));
    pStateManager_->AddState(new MainSwordWeaponCombo3(pStateManager_));
    pStateManager_->ChangeState("");
    pStateManager_->Initialize();

    seg_ = new SegmentCollider(XMFLOAT3(), XMVECTOR());
    seg_->SetValid(false);
    AddAttackCollider(seg_);

    pPolyLine_ = new PolyLine;
    pPolyLine_->Load("PolyImage/Sword.png");
}

void MainSwordWeapon::Update()
{

}

void MainSwordWeapon::Draw()
{
    if (!IsVisibled()) return;
   
    wandPos_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    
    XMFLOAT3 pPos = pPlayer_->GetPosition();
    seg_->SetCenter({ wandPos_.x - pPos.x, wandPos_.y - pPos.y, wandPos_.z - pPos.z });

    if (transform_.rotate_.x >= 90.0f || transform_.rotate_.x <= -90.0f) {
        transform_.rotate_.y *= -1.0f;
    }
    transform_.rotate_.y += pPlayer_->GetRotate().y;

    Transform t = transform_;
    t.position_ = wandPos_;
    Model::SetTransform(hModel_, t);
    Model::Draw(hModel_);

    pPolyLine_->Draw();
}

void MainSwordWeapon::Release()
{
    SAFE_RELEASE(pPolyLine_);
    SAFE_DELETE(pPolyLine_);

}

void MainSwordWeapon::OnAttackCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        EnemyBase* e = static_cast<EnemyBase*>(pTarget);
        e->ApplyDamage(damage_);
        e->SetAllColliderValid(false);
        e->SetKnockBack(MEDIUM, 2, 0.01f, pPlayer_->GetPosition());
        VFXManager::CreatVfxExplode1(wandPos_);
    }
}

void MainSwordWeapon::ChangeAttackState()
{
    atkEnd_ = false;
    pStateManager_->ChangeState("Combo1");

}

void MainSwordWeapon::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("");
    GameManager::GetEnemyManager()->ResetAllEnemyCollider();
    seg_->SetValid(false);

}

void MainSwordWeapon::CalcDamage()
{
    XMFLOAT3 tar = XMFLOAT3(transform_.rotate_.x, transform_.rotate_.y, 0.0f);
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
    vVec = XMVector3Normalize(vVec) * WEAPON_SIZE;
    seg_->SetVector(vVec);
    seg_->SetValid(true);

    XMStoreFloat3(&vec, vVec);
    vec = XMFLOAT3(wandPos_.x + vec.x, wandPos_.y + vec.y, wandPos_.z + vec.z);
    pPolyLine_->AddPosition(wandPos_, vec);

}

//--------------------state---------------------------------------------------

MainSwordWeaponCombo1::MainSwordWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void MainSwordWeaponCombo1::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    if (time_ >= ROTATE_FRAME[0] && time_ <= ROTATE_FRAME[1]) {
        float rRatio = (float)time_ / (float)ROTATE_FRAME[1];
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(rRatio);
        else if (p->GetCommand()->CmdWalk()) p->Rotate(rRatio);
    }

    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (p->GetCommand()->CmdAtk()) next_ = true;

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if(time_ >= ANIM_ATTACK_FRAME1[0] && time_ <= ANIM_ATTACK_FRAME1[1])
    m->CalcDamage();
    
    time_++;
    if (time_ >= COMBO_TIME1) {
        if (next_ == true) {
            GameManager::GetEnemyManager()->ResetAllEnemyCollider();
            owner_->ChangeState("Combo2");
        }
        else m->ResetState();
    }
}

void MainSwordWeaponCombo1::OnEnter()
{
    time_ = 0;
    next_ = false;
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    Model::SetAnimFrame(p->GetModelHandle(), ANIM_FRAME1[0], ANIM_FRAME1[1], 1.0f);

}

void MainSwordWeaponCombo1::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}

//---------------------------------------------

MainSwordWeaponCombo2::MainSwordWeaponCombo2(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void MainSwordWeaponCombo2::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    if (time_ >= ROTATE_FRAME[0] && time_ <= ROTATE_FRAME[1]) {
        float rRatio = (float)time_ / (float)ROTATE_FRAME[1];
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(rRatio);
        else if (p->GetCommand()->CmdWalk()) p->Rotate(rRatio);
    }

    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (p->GetCommand()->CmdAtk()) next_ = true;

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if (time_ >= ANIM_ATTACK_FRAME2[0] && time_ <= ANIM_ATTACK_FRAME2[1])
    m->CalcDamage();

    time_++;
    if (time_ >= COMBO_TIME2) {
        GameManager::GetEnemyManager()->ResetAllEnemyCollider();
        if (next_ == true) {
            GameManager::GetEnemyManager()->ResetAllEnemyCollider();
            owner_->ChangeState("Combo3");
        }
        else m->ResetState();
    }
}

void MainSwordWeaponCombo2::OnEnter()
{
    time_ = 0;
    next_ = false;
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    Model::SetAnimFrame(p->GetModelHandle(), ANIM_FRAME2[0], ANIM_FRAME2[1], 1.0f);

}

void MainSwordWeaponCombo2::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}

//---------------------------------------------

MainSwordWeaponCombo3::MainSwordWeaponCombo3(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void MainSwordWeaponCombo3::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    if (time_ >= ROTATE_FRAME[0] && time_ <= ROTATE_FRAME[1]) {
        float rRatio = (float)time_ / (float)ROTATE_FRAME[1];
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(rRatio);
        else if (p->GetCommand()->CmdWalk()) p->Rotate(rRatio);
    }
    
    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (p->GetCommand()->CmdAtk()) next_ = true;

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if (time_ >= ANIM_ATTACK_FRAME3[0] && time_ <= ANIM_ATTACK_FRAME3[1])
    m->CalcDamage();

    time_++;
    if (time_ >= COMBO_TIME3) {
        GameManager::GetEnemyManager()->ResetAllEnemyCollider();
        if (next_ == true) {
            GameManager::GetEnemyManager()->ResetAllEnemyCollider();
            owner_->ChangeState("Combo1");
        }
        else m->ResetState();
    }
}

void MainSwordWeaponCombo3::OnEnter()
{
    time_ = 0;
    next_ = false;
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    Model::SetAnimFrame(p->GetModelHandle(), ANIM_FRAME3[0], ANIM_FRAME3[1], 1.0f);

}

void MainSwordWeaponCombo3::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}