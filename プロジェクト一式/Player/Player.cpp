#include "Player.h"
#include "Aim.h"
#include "PlayerWeapon.h"
#include "LifeManager.h"
#include "PlayerData.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/Global.h"
#include "../Engine/SphereCollider.h"
#include "../State/StateManager.h"
#include "../State/PlayerState.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"
#include "../Scene/StageBase.h"
#include "../Other/VFXManager.h"
#include "../Other/InputManager.h"
#include "../Animation/AnimationController.h"

#include "../Engine/Text.h"

namespace {
    const float moveSpeed = 0.08f;          //移動スピード
    const float stopGradually = 0.21f;      //移動スピードの加減の値止まるとき
    const float moveGradually = 0.09f;      //移動スピードの加減の値移動時
    const float maxMoveSpeed = 0.7f;        //最大移動スピード
    const float rotateRatio = 0.1f;         //通常時のRotateRatio
    const float avoRotateRatio = 1.0f;      //回避時のRotateRatio
    const int DEF_LIFE_MAX = 50;
   
    //デバッグ用
    bool isCollider = true; //当たり判定するかどうか

}

Player::Player(GameObject* parent)
    : Character(parent, "Player"), hModel_(-1), pAim_(nullptr), pStateManager_(nullptr), pPlayerWeapon_(nullptr),
    pAnimationController_(nullptr), pCollider_{nullptr, nullptr}, playerMovement_(0,0,0), gradually_(0.0f)
{
}

Player::~Player()
{
    SAFE_DELETE(pStateManager_);
    SAFE_DELETE(pPlayerWeapon_);
    SAFE_DELETE(pAnimationController_);

}

void Player::Initialize()
{
    GameManager::SetPlayer(this);
    hModel_ = Model::Load("Model/Fiter.fbx");
    assert(hModel_ >= 0);

    transform_.rotate_.y = 180.0f;
    transform_.position_ = GameManager::GetStage()->GetStartPosition();

    LifeManager::SetLife(DEF_LIFE_MAX - PlayerData::GetReceiveDamage(), DEF_LIFE_MAX);
    SetHP(DEF_LIFE_MAX - PlayerData::GetReceiveDamage());
    SetMaxHP(DEF_LIFE_MAX);
    SetBodyWeight(0.1f);
    SetBodyRange(0.3f);

    //アニメーションデータのセットフレームはヘッダに書いてる
    pAnimationController_ = new AnimationController(hModel_, this);
    for (int i = 0; i < (int)PLAYER_ANIMATION::MAX; i++) pAnimationController_->AddAnim(PLAYER_ANIMATION_DATA[i][0], PLAYER_ANIMATION_DATA[i][1]);

    pAim_ = Instantiate<Aim>(this);
    pPlayerWeapon_ = new PlayerWeapon(this);
    pPlayerWeapon_->SetPlayerDataWeapon();

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new PlayerWait(pStateManager_));
    pStateManager_->AddState(new PlayerWeaponChange(pStateManager_));
    pStateManager_->AddState(new PlayerWalk(pStateManager_));
    pStateManager_->AddState(new PlayerAvo(pStateManager_));
    pStateManager_->AddState(new PlayerAtk(pStateManager_));
    pStateManager_->AddState(new PlayerSubAtk(pStateManager_));
    pStateManager_->AddState(new PlayerHear(pStateManager_));
    pStateManager_->AddState(new PlayerDead(pStateManager_));
    pStateManager_->AddState(new PlayerAppear(pStateManager_));
    pStateManager_->AddState(new PlayerDisAppear(pStateManager_));
    pStateManager_->ChangeState("Appear");
    pStateManager_->Initialize();

    pCollider_[1] = new SphereCollider(XMFLOAT3(0.0f, 1.1f, 0.0f), 0.2f);
    pCollider_[0] = new SphereCollider(XMFLOAT3(0.0f, 0.4f, 0.0f), 0.26f);
    AddCollider(pCollider_[0]);
    AddCollider(pCollider_[1]);

}

void Player::Update()
{
    Character::Update();
    pAnimationController_->Update();
    pStateManager_->Update();

    if (InputManager::IsCmdDown(InputManager::TARGET)) pAim_->SetTargetEnemy();
    if (isCollider) GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.1f, GetBodyRange());

    //デバッグ用
    if (Input::IsKeyDown(DIK_T)) isCollider = !isCollider;

}

#include "../Engine/FbxParts.h"
void Player::Draw()
{
    //OrientWarping
    static float targetY = 0.0f;
    static const float rotMaxY = 60.0f;
    static const float rotMaxYD = 100.0f;
    static const float rotRatio = 0.2f;
    if (transform_.rotate_.y <= -180.0f) transform_.rotate_.y += 360.0f;
    if (transform_.rotate_.y >= 180.0f) transform_.rotate_.y -= 360.0f;
    if (pStateManager_->GetName() == "Walk" && pAim_->IsTarget()) {
        //移動中なら移動方向から向く方向計算
        float rotYYY = 0.0f;
        if (InputManager::CmdWalk()) rotYYY = CalculationRotate(GetInputMove()).y - transform_.rotate_.y;
        else rotYYY = 0.0f;

        //計算結果を使える値に変換
        if (rotYYY >= 180.0f) rotYYY = -180.0f - (180.0f - rotYYY);
        else if (rotYYY <= -180.0f) rotYYY = 180.0f + (180.0f + rotYYY);

        //後ろ向きの時は向きを反転させる
        if (rotYYY >= rotMaxYD || rotYYY <= -rotMaxYD) {
            if (rotYYY > 0) rotYYY -= 180.0f;
            else rotYYY += 180.0f;
        }
        //回転量制限
        if (rotYYY >= rotMaxY) rotYYY = rotMaxY;
        else if (rotYYY <= -rotMaxY) rotYYY = -rotMaxY;

        targetY = targetY + ((rotYYY - targetY) * rotRatio);
        Model::GetFbx(hModel_)->GetFbxParts().at(0)->orientRotY = targetY;
    }
    else {
        targetY = targetY + ((0.0f - targetY) * rotRatio);
        Model::GetFbx(hModel_)->GetFbxParts().at(0)->orientRotY = targetY;
    }
    
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
    pPlayerWeapon_->DrawWeapon();

    //デバッグ用
    CollisionDraw();
}

void Player::Release()
{
}

void Player::Damage()
{
    if (pStateManager_->GetName() == "Dead") return;
    LifeManager::Damage(0);
}

void Player::Dead()
{
    if (GetStateManager()->GetName() != "Dead") GetStateManager()->ChangeState("Dead");
}

void Player::SmallKnockBack()
{
}

void Player::MediumKnockBack()
{
    if (GetStateManager()->GetName() != "Hear") GetStateManager()->ChangeState("Hear");
    GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::DAMAGE_SMALL, 0.3f);

}

void Player::LargeKnockBack()
{
    if (GetStateManager()->GetName() != "Hear") GetStateManager()->ChangeState("Hear");
    GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::DAMAGE_MID, 0.3f);

}

//private関数：Rotateの計算する--------------------------------
void Player::CalcRotate(XMFLOAT3 pos, float ratio)
{
    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b = XMFLOAT2(pos.x, pos.z );
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * ratio);
}

void Player::TargetRotate(XMFLOAT3 pos, float ratio) { CalcRotate(XMFLOAT3(pos.x - transform_.position_.x, 0.0f, pos.z - transform_.position_.z), ratio); }
void Player::Rotate() { Rotate(rotateRatio); }
void Player::Rotate(float ratio) { CalcRotate(GetInputMove(), ratio); }
void Player::AimTargetRotate() { AimTargetRotate(rotateRatio); }
void Player::AimTargetRotate(float ratio) { 
    XMFLOAT3 pos = pAim_->GetTargetPos();
    CalcRotate(XMFLOAT3(pos.x - transform_.position_.x, 0.0f, pos.z - transform_.position_.z), ratio);
}
//-----------------------------------------------Rotate

XMFLOAT3 Player::GetInputMove()
{
    XMFLOAT3 fMove = { 0,0,0 };
    if (InputManager::CmdWalk()) {
        gradually_ = moveGradually;

        XMFLOAT3 aimDirection = pAim_->GetAimDirection();
        if (InputManager::IsCmd(InputManager::MOVE_UP)) {
            fMove.x += aimDirection.x;
            fMove.z += aimDirection.z;
        }
        if (InputManager::IsCmd(InputManager::MOVE_LEFT)) {
            fMove.x -= aimDirection.z;
            fMove.z += aimDirection.x;
        }
        if (InputManager::IsCmd(InputManager::MOVE_DOWN)) {
            fMove.x -= aimDirection.x;
            fMove.z -= aimDirection.z;
        }
        if (InputManager::IsCmd(InputManager::MOVE_RIGHT)) {
            fMove.x += aimDirection.z;
            fMove.z -= aimDirection.x;
        }
    }

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);

    return fMove;
}

void Player::FrontMove(float f)
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    vMove = XMVector3Normalize(vMove + XMLoadFloat3(&playerMovement_));
    XMFLOAT3 move{};
    XMStoreFloat3(&move, vMove);

    transform_.position_.x += ((move.x * moveSpeed) * f);
    transform_.position_.z += ((move.z * moveSpeed) * f);
    SetMovement((XMLoadFloat3(&move) * moveSpeed) * f);
}

void Player::BackMove(float f)
{
    XMVECTOR vMove = { 0.0, 0.0, -1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    vMove = XMVector3Normalize(vMove + XMLoadFloat3(&playerMovement_));
    XMFLOAT3 move{};
    XMStoreFloat3(&move, vMove);

    transform_.position_.x += ((move.x * moveSpeed) * f);
    transform_.position_.z += ((move.z * moveSpeed) * f);
    SetMovement((XMLoadFloat3(&move) * moveSpeed) * f);
}

void Player::Move(float f)
{
    transform_.position_.x += ((playerMovement_.x * moveSpeed) * f);
    transform_.position_.z += ((playerMovement_.z * moveSpeed) * f);
    SetMovement((XMLoadFloat3(&playerMovement_) * moveSpeed) * f);
}

XMVECTOR Player::GetDirectionVec()
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    return vMove;
}

void Player::ResetMovement()
{
    Character::ResetMovement();
    playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Player::CalcMove()
{
    gradually_ = stopGradually;

    XMFLOAT3 fMove = GetInputMove();
    XMFLOAT3 move = { ((fMove.x - playerMovement_.x) * gradually_) , 0.0f , ((fMove.z - playerMovement_.z) * gradually_) };
    playerMovement_ = { playerMovement_.x + move.x , 0.0f , playerMovement_.z + move.z };

    //MaxSpeed超えていたら正規化・MaxSpeedの値にする
    float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerMovement_)));
    if (currentSpeed > maxMoveSpeed) {
        XMVECTOR vMove;
        vMove = XMLoadFloat3(&playerMovement_);
        vMove = XMVector3Normalize(vMove);
        vMove *= maxMoveSpeed;
        XMStoreFloat3(&playerMovement_, vMove);
    }
}

void Player::CalcNoMove()
{
    XMFLOAT3 move = { 0,0,0 };
    gradually_ = stopGradually;
    move = { ((move.x - playerMovement_.x) * gradually_) , 0.0f , ((move.z - playerMovement_.z) * gradually_) };
    playerMovement_ = { playerMovement_.x + move.x , 0.0f , playerMovement_.z + move.z };
}

void Player::Avo()
{
    //動いている場合
    if (InputManager::CmdWalk()) {
        CalcMove();
        Rotate(avoRotateRatio);
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
        GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::RORING, 0.2f);

        GetSphereCollider(0)->SetValid(false);
        GetSphereCollider(1)->SetValid(false);
        SetAllColliderValid(false);
    }
    //動いていない・ターゲット状態
    else if(pAim_->IsTarget()) {
        AimTargetRotate(1.0f);
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed * -1.0f);
        GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::BACK_STEP, 0.2f);
    
    }
    //動いていない・ターゲットもしていない
    else {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
        GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::RORING, 0.2f);

        GetSphereCollider(0)->SetValid(false);
        GetSphereCollider(1)->SetValid(false);

    }

}