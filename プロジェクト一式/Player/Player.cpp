#include "Player.h"
#include "Aim.h"
#include "PlayerWeapon.h"
#include "LifeManager.h"
#include "PlayerData.h"
#include "../VFXManager.h"
#include "../InputManager.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/Global.h"
#include "../Engine/SphereCollider.h"
#include "../State/StateManager.h"
#include "../State/PlayerState.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"
#include "../AnimationController.h"
#include "../Weapon/BulletBase.h"
#include "../Character/CharacterManager.h"
#include "../Scene/StageBase.h"

#include "../Engine/Text.h"

namespace {
    const float stopGradually = 0.21f;      //移動スピードの加減の値止まるとき
    const float moveGradually = 0.09f;      //移動スピードの加減の値移動時
    const float maxMoveSpeed = 0.7f;        //最大移動スピード
    const float avoRotateRatio = 1.0f;      //回避時のRotateRatio
    const int lifeMax = 50;
    const int invincible = 60;
    const int APPER_TIME = 60;

    bool isCollider = true; //当たり判定するかどうか
    Text* pText = new Text;

}

Player::Player(GameObject* parent)
    : Character(parent, "Player"), hModel_(-1), pAim_(nullptr), pStateManager_(nullptr), pPlayerWeapon_(nullptr),
    pAnimationController_(nullptr), pCollider_{nullptr, nullptr},
    moveSpeed_(0.0f), rotateRatio_(0.0f), playerMovement_(0,0,0), apperPos_(0,0,0), time_(0), gradually_(0.0f)
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

    moveSpeed_ = 0.08f;
    rotateRatio_ = 0.2f;
    bodyWeight_ = 0.1f;
    bodyRange_ = 0.3f;
    time_ = APPER_TIME;

    //アニメーションデータのセットフレームはヘッダに書いてる
    pAnimationController_ = new AnimationController(hModel_);
    for (int i = 0; i < PLAYER_ANIMATION::MAX; i++) pAnimationController_->AddAnime(PLAYER_ANIMATION_DATA[i].startFrame, PLAYER_ANIMATION_DATA[i].endFrame);

    pAim_ = Instantiate<Aim>(this);
    pPlayerWeapon_ = new PlayerWeapon(this);
    pPlayerWeapon_->SetPlayerDataWeapon();
    LifeManager::SetLife(lifeMax - PlayerData::GetReceiveDamage(), lifeMax);
    LifeManager::SetInvincible(invincible);

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

    pCollider_[1] = new SphereCollider(XMFLOAT3(0.0f, 0.95f, 0.0f), 0.25f);
    pCollider_[0] = new SphereCollider(XMFLOAT3(0.0f, 0.4f, 0.0f), 0.3f);
    AddCollider(pCollider_[0]);
    AddCollider(pCollider_[1]);

    apperPos_ = transform_.position_;
    transform_.position_.y += (time_ * 0.5f);

    pText->Initialize();

}

void Player::Update()
{
    Character::Update();

    pAnimationController_->Update();
    pStateManager_->Update();

    if (InputManager::IsCmdDown(InputManager::TARGET)) pAim_->SetTargetEnemy();
    if (isCollider) GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.1f, GetBodyRange());
    ReflectCharacter();

    //デバッグ用
    if (Input::IsKey(DIK_3)) { LifeManager::Damage(30); }
    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;
    if (Input::IsKeyDown(DIK_T)) isCollider = !isCollider;

}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
    pPlayerWeapon_->DrawWeapon();

    //デバッグ用
    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.y);
    pText->Draw(30, 110, (int)transform_.position_.z);
    CollisionDraw();
}

void Player::Release()
{
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
void Player::Rotate() { Rotate(rotateRatio_); }
void Player::Rotate(float ratio) { CalcRotate(GetInputMove(), ratio); }
void Player::AimTargetRotate() { AimTargetRotate(rotateRatio_); }
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

    transform_.position_.x += ((move.x * moveSpeed_) * f);
    transform_.position_.z += ((move.z * moveSpeed_) * f);
    SetMovement((XMLoadFloat3(&move) * moveSpeed_) * f);
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

    transform_.position_.x += ((move.x * moveSpeed_) * f);
    transform_.position_.z += ((move.z * moveSpeed_) * f);
    SetMovement((XMLoadFloat3(&move) * moveSpeed_) * f);
}

void Player::Move(float f)
{
    transform_.position_.x += ((playerMovement_.x * moveSpeed_) * f);
    transform_.position_.z += ((playerMovement_.z * moveSpeed_) * f);
    SetMovement((XMLoadFloat3(&playerMovement_) * moveSpeed_) * f);
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
    movement_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
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
        GetAnimationController()->SetNextAnime(2, 0.2f);

        GetSphereCollider(0)->SetValid(false);
        GetSphereCollider(1)->SetValid(false);
        SetAllColliderValid(false);
    }
    //動いていない・ターゲット状態
    else if(pAim_->IsTarget()) {
        AimTargetRotate(1.0f);
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed * -1.0f);
        GetAnimationController()->SetNextAnime(3, 0.2f);
    
    }
    //動いていない・ターゲットもしていない
    else {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
        GetAnimationController()->SetNextAnime(2, 0.2f);

        GetSphereCollider(0)->SetValid(false);
        GetSphereCollider(1)->SetValid(false);

    }

}