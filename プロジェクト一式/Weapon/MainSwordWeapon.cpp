#include "MainSwordWeapon.h"
#include "../InputManager.h"
#include "../AudioManager.h"
#include "../VFXManager.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Engine/PolyLine.h"
#include "../Engine/Global.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/Model.h"
#include "../State/StateManager.h"
#include "../Enemy/EnemyManager.h"

namespace {
    static const int POLY_DRAW_TIME = 10;
    static const int POLY_SMOOTH = 2;

    static const float WEAPON_SIZE = 1.2f;
    static const float BLEND_= 1.2f;
    static const float MOVE_SPEED = 0.4f;
    static const float ROTATE_RATIO = 0.2f;

    DamageInfo damage1(nullptr, "Sword", 20);
    DamageInfo damage2(nullptr, "Sword", 20);
    DamageInfo damage3(nullptr, "Sword", 30);
    KnockBackInfo knock1(KNOCK_TYPE::MEDIUM, 2, 0.1f, XMFLOAT3());
    KnockBackInfo knock2(KNOCK_TYPE::MEDIUM, 2, 0.1f, XMFLOAT3());
    KnockBackInfo knock3(KNOCK_TYPE::MEDIUM, 2, 0.2f, XMFLOAT3());
    CameraShakeInfo shakeInfo(2, 0.3f, 1.0f, 0.3f, 1.0f);

    //ブレンドを下げる値
    static const float ANIMATION_DECREASE = 0.3f;

    //回転フレーム
    static const int ROTATE_FRAME[2] = { 3, 10 };

    //攻撃Audio再生フレーム
    static const int ANIM_AUDIO_FRAME1 = 20;
    static const int ANIM_AUDIO_FRAME2 = 20;
    static const int ANIM_AUDIO_FRAME3 = 5;

    //攻撃判定フレーム
    static const int ANIM_ATTACK_FRAME1[2] = { 17, 30 };
    static const int ANIM_ATTACK_FRAME2[2] = { 15, 25 };
    static const int ANIM_ATTACK_FRAME3[2] = { 3, 20 };

    //キャンセルフレーム / 後ろのデータはReadyNextでも使う
    static const int ANIM_CHANGE_FRAME1[2] = { 12, 40 };
    static const int ANIM_CHANGE_FRAME2[2] = { 10, 30 };
    static const int ANIM_CHANGE_FRAME3[2] = { 1, 20 };

}

MainSwordWeapon::MainSwordWeapon(GameObject* parent)
	: WeaponBase(parent, "MainSwordWeapon"), polyCreatTime_(0), wandPos_(0, 0, 0), direction_(0,0,0),
    pPlayer_(nullptr), pPolyLine_(nullptr), pDamageController_(nullptr), sphere_{ nullptr, nullptr }
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

    pDamageController_ = new DamageController();
    damage1.owner = pPlayer_;
    damage2.owner = pPlayer_;
    damage3.owner = pPlayer_;

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new MainSwordWeaponCombo1(pStateManager_));
    pStateManager_->AddState(new MainSwordWeaponCombo2(pStateManager_));
    pStateManager_->AddState(new MainSwordWeaponCombo3(pStateManager_));
    pStateManager_->ChangeState("");
    pStateManager_->Initialize();

    for (int i = 0; i < 2; i++) {
        sphere_[i] = new SphereCollider(XMFLOAT3(), 0.25f);
        sphere_[i]->SetValid(false);
        AddAttackCollider(sphere_[i]);
    }
    

    pPolyLine_ = new PolyLine;
    pPolyLine_->Load("PolyImage/Sword.png");
    pPolyLine_->SetLength(POLY_DRAW_TIME);
    pPolyLine_->SetSmooth(POLY_SMOOTH);
}

void MainSwordWeapon::Update()
{
    //ポリゴンを後ろから消してく
    if (polyCreatTime_ > 0) {
        polyCreatTime_--;
        pPolyLine_->ClearLastPositions();
    }

}

void MainSwordWeapon::Draw()
{
    if (!IsVisibled()) return;
   
    //座標の取得と剣先のDirection計算
    XMFLOAT3 preWandPos = wandPos_;
    wandPos_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    direction_ = Float3Sub(wandPos_, preWandPos);
    direction_ = Float3Normalize(direction_);

    //回転軸を取得
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_.y += pPlayer_->GetRotate().y;

    Transform t = transform_;
    t.position_ = wandPos_;
    Model::SetTransform(hModel_, t);
    Model::Draw(hModel_);
   
    pPolyLine_->Draw();
    CollisionDraw();
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

        //攻撃入ったらリストに追加
        if (e->ApplyDamageWithList(damage, knock)) {
            pDamageController_->AddAttackList(e);
            
            VFXManager::CreatVfxSwordSlash(wandPos_, direction_);
            pPlayer_->GetAim()->SetCameraShakeDirection(XMLoadFloat3(&direction_));
            pPlayer_->GetAim()->SetCameraShake(shakeInfo);
        }
    }
}

void MainSwordWeapon::ChangeAttackState()
{
    isAtkEnd_ = false;
    isCancellable_ = false;
    pStateManager_->ChangeState("Combo1");
}

void MainSwordWeapon::ResetState()
{
    isAtkEnd_ = true;
    isCancellable_ = false;
    pStateManager_->ChangeState("");
    SetAllAttackColliderValid(false);
    SetPolyCreatTime();
}

void MainSwordWeapon::CalcSwordTrans()
{
    XMFLOAT3 target = CalculationDirection(transform_.rotate_);

    SetAllAttackColliderValid(true);
    XMFLOAT3 vec = Float3Multiply(Float3Normalize(target), WEAPON_SIZE);

    //剣先の座標計算ワールド
    XMFLOAT3 swordTip = Float3Add(wandPos_, vec);
    
    //ワールドの座標をPolyに
    pPolyLine_->AddPosition(wandPos_, swordTip);

    //ローカルにしてCollider設定
    XMFLOAT3 pPos = pPlayer_->GetPosition();
    swordTip = Float3Sub(swordTip, pPos);
    sphere_[0]->SetCenter(swordTip);

    //中間のCollider
    vec = Float3Multiply(Float3Normalize(target), WEAPON_SIZE * 0.5f);
    swordTip = Float3Add(wandPos_, vec);
    swordTip = Float3Sub(swordTip, pPos);
    sphere_[1]->SetCenter(swordTip);
}

void MainSwordWeapon::SetPolyCreatTime()
{
    polyCreatTime_ = POLY_DRAW_TIME;
}

void MainSwordWeapon::DamageInfoReset()
{
    pDamageController_->ResetAttackList();
}

void MainSwordWeapon::SetDamageInfoCombo1()
{
    pDamageController_->SetCurrentDamage(damage1);
    pDamageController_->SetCurrentKnockBackInfo(knock1);
}

void MainSwordWeapon::SetDamageInfoCombo2()
{
    pDamageController_->SetCurrentDamage(damage2);
    pDamageController_->SetCurrentKnockBackInfo(knock2);
}

void MainSwordWeapon::SetDamageInfoCombo3()
{
    pDamageController_->SetCurrentDamage(damage3);
    pDamageController_->SetCurrentKnockBackInfo(knock3);
}

//--------------------state---------------------------------------------------

MainSwordWeaponCombo1::MainSwordWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void MainSwordWeaponCombo1::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    int comboTime = p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK1).endFrame -
                    p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK1).startFrame;
    time_++;

    if (time_ >= ROTATE_FRAME[0] && time_ <= ROTATE_FRAME[1]) {
        float rRatio = (float)time_ / (float)ROTATE_FRAME[1];
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(rRatio);
        else if (InputManager::CmdWalk()) p->Rotate(rRatio);
    }

    p->CalcNoMove();
    p->FrontMove((1.0f - float(time_) / (float)comboTime) * MOVE_SPEED);
    m->SetAllAttackColliderValid(false);
    
    //攻撃判定
    if(time_ >= ANIM_ATTACK_FRAME1[0] && time_ <= ANIM_ATTACK_FRAME1[1]) m->CalcSwordTrans();
    //攻撃終了判定
    if (time_ == ANIM_ATTACK_FRAME1[1]) m->SetPolyCreatTime();
    //Audio再生
    if (time_ == ANIM_AUDIO_FRAME1) AudioManager::Play(AUDIO_ID::SWORD_WIELD_1);
    
    //入力
    if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) next_ = true;
    if (InputManager::IsCmdDown(InputManager::AVO)) next_ = false;

    //キャンセルとReadyNext
    m->SetCancellable(false);
    m->SetNextReady(false); 
    if (time_ <= ANIM_CHANGE_FRAME1[0]) m->SetCancellable(true);
    else if (time_ >= ANIM_CHANGE_FRAME1[1]) {
        if (next_) {
            owner_->ChangeState("Combo2");
            return;
        }
        else {
            m->SetCancellable(true);
            m->SetNextReady(true);
        }
    }

    if (time_ >= comboTime) {
        if (next_ == true) owner_->ChangeState("Combo2");
        else m->ResetState();
    }
}

void MainSwordWeaponCombo1::OnEnter()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    p->GetAnimationController()->SetNextAnime((int)PLAYER_ANIMATION::ATTACK1, ANIMATION_DECREASE);
    m->SetDamageInfoCombo1();
    time_ = 0;
    next_ = false;
}

void MainSwordWeaponCombo1::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->DamageInfoReset();
}

//---------------------------------------------

MainSwordWeaponCombo2::MainSwordWeaponCombo2(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void MainSwordWeaponCombo2::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    int comboTime = p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK2).endFrame -
                    p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK2).startFrame;
    time_++;

    if (time_ >= ROTATE_FRAME[0] && time_ <= ROTATE_FRAME[1]) {
        float rRatio = (float)time_ / (float)ROTATE_FRAME[1];
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(rRatio);
        else if (InputManager::CmdWalk()) p->Rotate(rRatio);
    }

    p->CalcNoMove();
    p->FrontMove((1.0f - float(time_) / (float)comboTime) * MOVE_SPEED);
    m->SetAllAttackColliderValid(false);

    //攻撃判定
    if (time_ >= ANIM_ATTACK_FRAME2[0] && time_ <= ANIM_ATTACK_FRAME2[1]) m->CalcSwordTrans();
    //攻撃終了判定
    if (time_ == ANIM_ATTACK_FRAME2[1]) m->SetPolyCreatTime();
    //Audio再生
    if (time_ == ANIM_AUDIO_FRAME2) AudioManager::Play(AUDIO_ID::SWORD_WIELD_2);

    //入力
    if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) next_ = true;
    if (InputManager::IsCmdDown(InputManager::AVO)) next_ = false;

    //キャンセルとReadyNext
    m->SetCancellable(false);
    m->SetNextReady(false);
    if (time_ <= ANIM_CHANGE_FRAME2[0]) m->SetCancellable(true);
    else if (time_ >= ANIM_CHANGE_FRAME2[1]) {
        if (next_) {
            owner_->ChangeState("Combo3");
            return;
        }
        else {
            m->SetCancellable(true);
            m->SetNextReady(true);
        }
    }
   
    if (time_ >= comboTime) {
        if (next_ == true) owner_->ChangeState("Combo3");
        else m->ResetState();
    }
}

void MainSwordWeaponCombo2::OnEnter()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    p->GetAnimationController()->SetNextAnime((int)PLAYER_ANIMATION::ATTACK2, ANIMATION_DECREASE);
    m->SetDamageInfoCombo2();
    time_ = 0;
    next_ = false;
}

void MainSwordWeaponCombo2::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->DamageInfoReset();
}

//---------------------------------------------

MainSwordWeaponCombo3::MainSwordWeaponCombo3(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void MainSwordWeaponCombo3::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    int comboTime = p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK3).endFrame -
                    p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::ATTACK3).startFrame;
    time_++;

    if (time_ >= ROTATE_FRAME[0] && time_ <= ROTATE_FRAME[1]) {
        float rRatio = (float)time_ / (float)ROTATE_FRAME[1];
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(rRatio);
        else if (InputManager::CmdWalk()) p->Rotate(rRatio);
    }
    
    p->CalcNoMove();
    p->FrontMove((1.0f - float(time_) / (float)comboTime) * MOVE_SPEED);
    m->SetAllAttackColliderValid(false);

    //攻撃判定
    if (time_ >= ANIM_ATTACK_FRAME3[0] && time_ <= ANIM_ATTACK_FRAME3[1]) m->CalcSwordTrans();
    //攻撃終了判定
    if (time_ == ANIM_ATTACK_FRAME3[1]) m->SetPolyCreatTime();
    //Audio再生
    if (time_ == ANIM_AUDIO_FRAME3) AudioManager::Play(AUDIO_ID::SWORD_WIELD_3);
    
    //入力
    if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) next_ = true;
    if (InputManager::IsCmdDown(InputManager::AVO)) next_ = false;

    //キャンセルとReadyNext
    m->SetCancellable(false);
    m->SetNextReady(false);
    if (time_ <= ANIM_CHANGE_FRAME3[0]) m->SetCancellable(true);
    else if (time_ >= ANIM_CHANGE_FRAME3[1]) {
        if (next_) {
            owner_->ChangeState("Combo1");
            return;
        }
        else {
            m->SetCancellable(true);
            m->SetNextReady(true);
        }
    }

    if (time_ >= comboTime) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else m->ResetState();
    }
}

void MainSwordWeaponCombo3::OnEnter()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    p->GetAnimationController()->SetNextAnime((int)PLAYER_ANIMATION::ATTACK3, 1.0f, ANIMATION_DECREASE);
    m->SetDamageInfoCombo3();
    time_ = 0;
    next_ = false;
}

void MainSwordWeaponCombo3::OnExit()
{
    MainSwordWeapon* m = static_cast<MainSwordWeapon*>(owner_->GetGameObject());
    m->DamageInfoReset();
}