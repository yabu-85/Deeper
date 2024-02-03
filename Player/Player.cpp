#include "Player.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/Global.h"
#include "Aim.h"
#include "../State/StateManager.h"
#include "../State/PlayerState.h"
#include "PlayerCommand.h"
#include "PlayerWeapon.h"
#include "../GameManager.h"
#include "../Stage/CreateStage.h"
#include "LifeManager.h"
#include "../VFXManager.h"

#include "../Engine/BoxCollider.h"
#include "../Engine/SphereCollider.h"
#include "../Stage/CollisionMap.h"
#include "../Engine/Text.h"

namespace {
    const float stopGradually = 0.25f;      //移動スピードの加減の値止まるとき
    const float moveGradually = 0.15f;      //移動スピードの加減の値移動時
    const float maxMoveSpeed = 1.0f;        //最大移動スピード
    const float avoRotateRatio = 0.92f;     //回避時のRotateRatio

    const int APPER_TIME = 60;
    const int HEAR_TIME = 30;

    bool isCollider = true; //当たり判定するかどうか、浮けるようにするための
    Text* pText = new Text;
}

void Player::ApperUpdate()
{
    time_--;
    transform_.position_.y -= 0.5f;

    XMFLOAT3 cPos = XMFLOAT3(apperPos_.x, apperPos_.y + 5.0f, apperPos_.z + 13.0f);
    pAim_->SetCompulsion(cPos, apperPos_);

    if (time_ <= 0) {
        state_ = MAIN_STATE::UPDATE;
        XMFLOAT3 pos = { transform_.position_.x, transform_.position_.y + 0.7f, transform_.position_.z };
        VFXManager::CreatVfxExplode1(pos);
    }
}

void Player::HearUpdate()
{
    time_--;
    if (time_ <= 0) {
        state_ = MAIN_STATE::UPDATE;
    }

}

void Player::DeadUpdate()
{
}

Player::Player(GameObject* parent)
    : Character(parent, "Player"), hModel_{-1, -1}, pAim_(nullptr), pStateManager_(nullptr), pCommand_(nullptr), pPlayerWeapon_(nullptr),
    moveSpeed_(0.0f), rotateRatio_(0.0f), playerMovement_(0,0,0), state_(MAIN_STATE::APPER), apperPos_(0,0,0), time_(0), gradually_(0.0f)
{
}

Player::~Player()
{
    SAFE_DELETE(pStateManager_);
    SAFE_DELETE(pCommand_);

}

void Player::Initialize()
{
    //モデルデータのロード
    hModel_[0] = Model::Load("Model/FiterTestUp.fbx");
    assert(hModel_[0] >= 0);

    hModel_[1] = Model::Load("Model/FiterTestDown.fbx");
    assert(hModel_[1] >= 0);
    transform_.rotate_.y += 180.0f;

    SetPosition(GameManager::GetCreateStage()->GetPlayerStartPos());
    moveSpeed_ = 0.08f;
    rotateRatio_ = 0.2f;
    bodyWeight_ = 5.1f;
    bodyRange_ = 0.3f;
    time_ = APPER_TIME;             
    transform_.scale_ = { 0.5f, 0.5f, 0.5f };

    pAim_ = Instantiate<Aim>(this);
    pCommand_ = new PlayerCommand();
    pPlayerWeapon_ = new PlayerWeapon(this);
    pPlayerWeapon_->SetPlayerDataWeapon();

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new PlayerWait(pStateManager_));
    pStateManager_->AddState(new PlayerWeaponChange(pStateManager_));
    pStateManager_->AddState(new PlayerWalk(pStateManager_));
    pStateManager_->AddState(new PlayerAvo(pStateManager_));
    pStateManager_->AddState(new PlayerAtk(pStateManager_));
    pStateManager_->AddState(new PlayerSubAtk(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    //BoxCollider* collider = new BoxCollider(XMFLOAT3(0.0f, 1.3f, 0.0f), XMFLOAT3(0.5f, 2.6f, 0.5f));
    //AddCollider(collider);
    SphereCollider* collid = new SphereCollider(XMFLOAT3(0.0f, 1.2f, 0.0f), 0.25f);
    AddCollider(collid);

    apperPos_ = transform_.position_;
    transform_.position_.y += (time_ * 0.5f);

    pText->Initialize();


}

void Player::Update()
{
    if (state_ == MAIN_STATE::APPER) ApperUpdate();
    else if (state_ == MAIN_STATE::HEAR) HearUpdate();
    else if (state_ == MAIN_STATE::DEAD) DeadUpdate();
    else {
        pCommand_->Update();
        pStateManager_->Update();
        if (pCommand_->CmdTarget()) pAim_->SetTargetEnemy();
    }
    
    //デバッグ用
    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;
    if (Input::IsKeyDown(DIK_T)) isCollider = !isCollider;
    if (isCollider) {
        CollisionMap* map = static_cast<CollisionMap*>(FindObject("CollisionMap"));
        map->CalcMapWall(transform_.position_, moveSpeed_);
    }

}

void Player::Draw()
{
    Model::SetTransform(hModel_[0], transform_);
    Model::Draw(hModel_[0]);
    Model::SetTransform(hModel_[1], transform_);
    Model::Draw(hModel_[1]);
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

void Player::OnCollision(GameObject* pTarget)
{
    std::string name = pTarget->GetObjectName();
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        Character* c = static_cast<Character*>(pTarget);
        ReflectCharacter(c);
    }

}

void Player::OnAttackCollision(GameObject* pTarget)
{
    std::string name = pTarget->GetObjectName();
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        EnemyBase* enemy = static_cast<EnemyBase*>(pTarget);
        LifeManager::Damage(enemy->GetAttackDamage());
        state_ = MAIN_STATE::HEAR;
        time_ = HEAR_TIME;
    }

}

void Player::Rotate()
{
    XMFLOAT3 move = GetInputMove();

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b = XMFLOAT2(move.x, move.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);

    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
}

void Player::Rotate(float ratio)
{
    XMFLOAT3 move = GetInputMove();

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b = XMFLOAT2(move.x, move.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);

    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * ratio);
}

void Player::AimTargetRotate()
{
    XMFLOAT3 tar = pAim_->GetTargetPos();
    float rotateY = transform_.rotate_.y;

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(rotateY)), cosf(XMConvertToRadians(rotateY)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b(transform_.position_.x - tar.x, transform_.position_.z - tar.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b)) * -1.0f;
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
}

void Player::AimTargetRotate(float ratio)
{
    XMFLOAT3 tar = pAim_->GetTargetPos();
    float rotateY = transform_.rotate_.y;

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(rotateY)), cosf(XMConvertToRadians(rotateY)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b(transform_.position_.x - tar.x, transform_.position_.z - tar.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b)) * -1.0f;
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * ratio);
}

XMFLOAT3 Player::GetInputMove()
{
    XMFLOAT3 fMove = { 0,0,0 };
    if (pCommand_->CmdWalk()) {
        gradually_ = moveGradually;

        XMFLOAT3 aimDirection = pAim_->GetAimDirection();
        if (pCommand_->CmdUp()) {
            fMove.x += aimDirection.x;
            fMove.z += aimDirection.z;
        }
        if (pCommand_->CmdLeft()) {
            fMove.x -= aimDirection.z;
            fMove.z += aimDirection.x;
        }
        if (pCommand_->CmdDown()) {
            fMove.x -= aimDirection.x;
            fMove.z -= aimDirection.z;
        }
        if (pCommand_->CmdRight()) {
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

void Player::InitAvo()
{
    //動いている場合は大体その方向
    if (pCommand_->CmdWalk()) {
        CalcMove();
        Rotate(avoRotateRatio);
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
    }
    //動いていなくて、ターゲット状態の時は向いている方向の逆に
    else if(pAim_->IsTarget()) {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed * -1.0);
    }
    //動いていなくて、かつターゲットもしていない時は向いている方向に
    else {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
    }

}