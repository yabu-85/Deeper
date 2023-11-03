#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "StateManager.h"
#include "PlayerState.h"
#include "PlayerCommand.h"
#include "TestWeaponMain.h"
#include "TestWeaponSub.h"

#include "Engine/Text.h"

namespace {
    float gradually = 0.1f;                 //移動スピードの加減の値
    const float stopGradually = 0.25f;      //移動スピードの加減の値止まるとき
    const float moveGradually = 0.15f;      //移動スピードの加減の値移動時
    const float maxMoveSpeed = 1.0f;        //最大移動スピード

    Text* pText = new Text;
    XMFLOAT3 prePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    
    float rotateTime = 0.0f;
}

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_{-1, -1}, pAim_(nullptr), playerMovement_{0,0,0}, moveVec_(0, 0, 0), pStateManager_(nullptr), pCommand_(nullptr),
    pMainWeapon_(nullptr), pSubWeapon_(nullptr)
{
    moveSpeed_ = 0.15f;
    rotateRatio_ = 0.2f;
}

Player::~Player()
{
}

void Player::Initialize()
{
    //モデルデータのロード
    hModel_[0] = Model::Load("Model/FiterTestUp.fbx");
    assert(hModel_[0] >= 0);

    hModel_[1] = Model::Load("Model/FiterTestDown.fbx");
    assert(hModel_[1] >= 0);
    transform_.rotate_.y += 180.0f;

    pMainWeapon_ = Instantiate<TestWeaponMain>(this);
    pMainWeapon_->SetOffsetScale(XMFLOAT3(0.1f, 1.0f, 0.1f));
    pMainWeapon_->SetOffsetRotate(XMFLOAT3(0.0f, 0.0f, 0.0f));
    pSubWeapon_ = Instantiate<TestWeaponSub>(this);
    pSubWeapon_->SetOffsetScale(XMFLOAT3(0.0f, 0.0f, 0.0f));
    pSubWeapon_->SetOffsetRotate(XMFLOAT3(0.0f, 180.0f, 0.0f));

    pAim_ = Instantiate<Aim>(this);
    pCommand_ = new PlayerCommand();

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new PlayerWait(pStateManager_));
    pStateManager_->AddState(new PlayerWalk(pStateManager_));
    pStateManager_->AddState(new PlayerAvo(pStateManager_));
    pStateManager_->AddState(new PlayerAtk(pStateManager_));
    pStateManager_->AddState(new PlayerSubAtk(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    pText->Initialize();
}

void Player::Update()
{
    pCommand_->Update();
    pStateManager_->Update();
    
    //エイムターゲット
    if (pCommand_->CmdTarget()) pAim_->SetTargetEnemy();

    if(pStateManager_->GetName() == "Wait" || pStateManager_->GetName() == "Walk")
    if (pCommand_->CmdCenterUp()) {
        if (pMainWeapon_ == nullptr) {
            pMainWeapon_ = Instantiate<TestWeaponMain>(this);
            pMainWeapon_->SetOffsetScale(XMFLOAT3(0.1f, 1.0f, 0.1f));
            pMainWeapon_->SetOffsetRotate(XMFLOAT3(0.0f, 0.0f, 0.0f));
        }
        else {
            pMainWeapon_->KillMe();
            pMainWeapon_ = nullptr;
        }
    }
    else if (pCommand_->CmdCenterDown()) {
        if (pSubWeapon_ == nullptr) {
            pSubWeapon_ = Instantiate<TestWeaponSub>(this);
            pSubWeapon_->SetOffsetScale(XMFLOAT3(0.0f, 0.0f, 0.0f));
            pSubWeapon_->SetOffsetRotate(XMFLOAT3(0.0f, 180.0f, 0.0f));
        }
        else {
            pSubWeapon_->KillMe();
            pSubWeapon_ = nullptr;
        }
    }

    //デバッグ用
    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;
}

void Player::Draw()
{
    Transform under = transform_;
    if (pAim_->IsTarget()) {
        float aimRotate = pAim_->GetRotate().y;
        float currentRotate = transform_.rotate_.y;
        float angleDifference = aimRotate - currentRotate;

        while (angleDifference > 180.0f) {
            angleDifference -= 360.0f;
        }
        while (angleDifference < -180.0f) {
            angleDifference += 360.0f;
        }

        angleDifference = abs(angleDifference);

        if (angleDifference < 90) {
            under.rotate_.y += 180.0f;
        }
    }
    Model::SetTransform(hModel_[1], under);
    Model::Draw(hModel_[1]);

    //ターゲット状態の場合はAim方向に強制
    if (pAim_->IsTarget()) {
        upTrans_ = transform_;
        upTrans_.rotate_.y = pAim_->GetRotate().y + 180.0f;
    }
    else upTrans_ = transform_;
    Model::SetTransform(hModel_[0], upTrans_);
    Model::Draw(hModel_[0]);

    if (pMainWeapon_ && !pMainWeapon_->IsDead()) pMainWeapon_->Draw();
    if (pSubWeapon_ && !pSubWeapon_->IsDead()) pSubWeapon_->Draw();

    //デバッグ用
    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.y);
    pText->Draw(30, 110, (int)transform_.position_.z);

}

void Player::Release()
{
}

//rotateRatioの比率で回転させる
void Player::Rotate()
{
    XMFLOAT2 b(transform_.position_.x - prePos.x, transform_.position_.z - prePos.z);
    XMVECTOR vB = XMLoadFloat2(&b);
    //移動量が０なら回転しない
    if (XMVectorGetX(XMVector3Length(vB)) == 0.0f) return;
    vB = XMVector2Normalize(XMLoadFloat2(&b));

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
}

void Player::Move(float f)
{
    prePos = transform_.position_;
    transform_.position_.x += ((playerMovement_.x * moveSpeed_) * f);
    transform_.position_.z += ((playerMovement_.z * moveSpeed_) * f);
}

bool Player::IsMoveKeyPushed(XMFLOAT3& key)
{
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();

    bool flag = false;
    if (pCommand_->CmdUp()) {
        key.x += aimDirection.x;
        key.z += aimDirection.z;
        flag = true;
    }
    if (pCommand_->CmdLeft()) {
        key.x -= aimDirection.z;
        key.z += aimDirection.x;
        flag = true;
    }
    if (pCommand_->CmdDown()) {
        key.x -= aimDirection.x;
        key.z -= aimDirection.z;
        flag = true;
    }
    if (pCommand_->CmdRight()) {
        key.x += aimDirection.z;
        key.z -= aimDirection.x;
        flag = true;
    }
    return flag;
}

bool Player::IsMove()
{
    return XMVector3NotEqual(XMLoadFloat3(&playerMovement_), XMVectorZero());
}

XMVECTOR Player::GetDirectionVec()
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    return vMove;
}

void Player::CalcMove()
{
    //fMoveの値を取る＆ graduallyを設定
    gradually = stopGradually;
    XMFLOAT3 fMove = { 0,0,0 };
    if (IsMoveKeyPushed(fMove)) gradually = moveGradually;

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);
    moveVec_ = fMove;

    fMove = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + fMove.x , 0.0f , playerMovement_.z + fMove.z };

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
    XMFLOAT3 fMove = { 0,0,0 };
    gradually = stopGradually;
    moveVec_ = fMove;
    fMove = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + fMove.x , 0.0f , playerMovement_.z + fMove.z };
}

void Player::InitAvo()
{
    if (pCommand_->CmdWalk()) {
        CalcMove();
        XMStoreFloat3(&playerMovement_, GetMoveVec());
    }
    else {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed * -1.0);
    }
    float preRatio = rotateRatio_;
    rotateRatio_ = 1.0f;
    Rotate();
    rotateRatio_ = preRatio;
}