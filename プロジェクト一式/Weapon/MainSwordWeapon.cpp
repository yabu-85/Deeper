#include "MainSwordWeapon.h"
#include "../InputManager.h"
#include "../AudioManager.h"
#include "../VFXManager.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
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

    static const int ATTACK_DAMAGE1 = 20;
    static const int ATTACK_DAMAGE2 = 20;
    static const int ATTACK_DAMAGE3 = 20;
    
    //‰ñ“]ƒtƒŒ[ƒ€
    static const int ROTATE_FRAME[2] = { 3, 10 };

    //UŒ‚AudioÄ¶ƒtƒŒ[ƒ€
    static const int ANIM_AUDIO_FRAME1 = 20;
    static const int ANIM_AUDIO_FRAME2 = 20;
    static const int ANIM_AUDIO_FRAME3 = 0;

    //UŒ‚”»’èƒtƒŒ[ƒ€
    static const int ANIM_ATTACK_FRAME1[2] = { 17, 33 };
    static const int ANIM_ATTACK_FRAME2[2] = { 15, 25 };
    static const int ANIM_ATTACK_FRAME3[2] = { 3, 12 };

}

MainSwordWeapon::MainSwordWeapon(GameObject* parent)
	: WeaponBase(parent, "MainSwordWeapon"), pPlayer_(nullptr), seg_(nullptr), wandPos_(0,0,0), pPolyLine_(nullptr), pDamageController_(nullptr)
{
    transform_.pParent_ = nullptr;
}

MainSwordWeapon::~MainSwordWeapon()
{
}

void MainSwordWeapon::Initialize()
{
    hModel_ = Model::Load("Model/Sword.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = static_cast<Player*>(GetParent());
    Model::GetBoneIndex(pPlayer_->GetModelHandle(), "Weapon", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);

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

    pDamageController_ = new DamageController();
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
    SAFE_DELETE(pDamageController_);

}

void MainSwordWeapon::OnAttackCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        EnemyBase* e = static_cast<EnemyBase*>(pTarget);
        DamageInfo damage(pDamageController_->GetCurrentDamage());
        KnockBackInfo knock(pDamageController_->GetCurrentKnockBackInfo());
        knock.pos = pPlayer_->GetPosition();

        //UŒ‚“ü‚Á‚½‚çƒŠƒXƒg‚É’Ç‰Á
        if (e->ApplyDamageWithList(damage, knock)) {
            VFXManager::CreatVfxExplode1(wandPos_);
            pDamageController_->AddAttackList(e);
        }
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
    seg_->SetValid(false);

}

void MainSwordWeapon::CalcSwordTrans()
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

void MainSwordWeapon::DamageInfoReset()
{
    pDamageController_->ResetAttackList();
}

void MainSwordWeapon::SetDamageInfoCombo1()
{
    DamageInfo damage(pPlayer_, "Sword", ATTACK_DAMAGE1);
    KnockBackInfo knock(KNOCK_TYPE::MEDIUM, 3, 0.2f, XMFLOAT3());
    pDamageController_->SetCurrentDamage(damage);
    pDamageController_->SetCurrentKnockBackInfo(knock);
}

void MainSwordWeapon::SetDamageInfoCombo2()
{
    DamageInfo damage(pPlayer_, "Sword", ATTACK_DAMAGE2);
    KnockBackInfo knock(KNOCK_TYPE::MEDIUM, 3, 0.2f, XMFLOAT3());
    pDamageController_->SetCurrentDamage(damage);
    pDamageController_->SetCurrentKnockBackInfo(knock);
}

void MainSwordWeapon::SetDamageInfoCombo3()
{
    DamageInfo damage(pPlayer_, "Sword", ATTACK_DAMAGE3);
    KnockBackInfo knock(KNOCK_TYPE::MEDIUM, 6, 0.3f, XMFLOAT3());
    pDamageController_->SetCurrentDamage(damage);
    pDamageController_->SetCurrentKnockBackInfo(knock);
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
        else if (InputManager::CmdWalk()) p->Rotate(rRatio);
    }

    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) next_ = true;

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if(time_ >= ANIM_ATTACK_FRAME1[0] && time_ <= ANIM_ATTACK_FRAME1[1])
    m->CalcSwordTrans();
    
    if (time_ == ANIM_AUDIO_FRAME1) AudioManager::Play(AUDIO_ID::SWORD_WIELD_1);

    time_++;
    int comboTime = p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK1).endFrame -
                    p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK1).startFrame;
    if (time_ >= comboTime) {
        if (next_ == true) {
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
    p->GetAnimationController()->SetNextAnime((int)PLAYER_ANIMATION::ATTACK1, 0.3f);

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->SetDamageInfoCombo1();
}

void MainSwordWeaponCombo1::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();
    m->DamageInfoReset();

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
        else if (InputManager::CmdWalk()) p->Rotate(rRatio);
    }

    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) next_ = true;

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if (time_ >= ANIM_ATTACK_FRAME2[0] && time_ <= ANIM_ATTACK_FRAME2[1])
    m->CalcSwordTrans();

    if (time_ == ANIM_AUDIO_FRAME2) AudioManager::Play(AUDIO_ID::SWORD_WIELD_2);

    time_++;
    int comboTime = p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK2).endFrame -
        p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK2).startFrame;
    if (time_ >= comboTime) {
        if (next_ == true) {
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
    p->GetAnimationController()->SetNextAnime((int)PLAYER_ANIMATION::ATTACK2, 0.3f);

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->SetDamageInfoCombo2();
}

void MainSwordWeaponCombo2::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();
    m->DamageInfoReset();

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
        else if (InputManager::CmdWalk()) p->Rotate(rRatio);
    }
    
    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) next_ = true;

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if (time_ >= ANIM_ATTACK_FRAME3[0] && time_ <= ANIM_ATTACK_FRAME3[1])
    m->CalcSwordTrans();

    if (time_ == ANIM_AUDIO_FRAME3) AudioManager::Play(AUDIO_ID::SWORD_WIELD_3);

    time_++;
    int comboTime = p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK3).endFrame -
        p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK3).startFrame; 
    if (time_ >= comboTime) {
        if (next_ == true) {
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
    p->GetAnimationController()->SetNextAnime((int)PLAYER_ANIMATION::ATTACK3, 1.0f, 1.0f);

    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->SetDamageInfoCombo3();
}

void MainSwordWeaponCombo3::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();
    m->DamageInfoReset();

}