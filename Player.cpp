#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), pAim_(nullptr), playerMovement_{0,0,0}
{
    moveSpeed_ = 0.15f;
}

Player::~Player()
{
}

void Player::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Model/PlayerModel.fbx");
    assert(hModel_ >= 0);

    pAim_ = Instantiate<Aim>(this);

}

void Player::Update()
{
    CalcMove();

    transform_.position_.x += ((playerMovement_.x * moveSpeed_));
    transform_.position_.z += ((playerMovement_.z * moveSpeed_));

}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void Player::Release()
{
}

void Player::CalcMove()
{
    XMFLOAT3 fMove = { 0,0,0 };
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();

    // PlayerクラスのMove関数内の一部
    if (Input::IsKey(DIK_W)) {
        fMove.x += aimDirection.x;
        fMove.z += aimDirection.z;
    }
    if (Input::IsKey(DIK_A)) {
        fMove.x -= aimDirection.z;
        fMove.z += aimDirection.x;
    }
    if (Input::IsKey(DIK_S)) {
        fMove.x -= aimDirection.x;
        fMove.z -= aimDirection.z;
    }
    if (Input::IsKey(DIK_D)) {
        fMove.x += aimDirection.z;
        fMove.z -= aimDirection.x;
    }

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);

    //滑る値？
    const float airMoveSpeed = 0.1f;

    fMove = { ((fMove.x - playerMovement_.x) * airMoveSpeed) , 0.0f , ((fMove.z - playerMovement_.z) * airMoveSpeed) };
    playerMovement_ = { playerMovement_.x + fMove.x , 0.0f , playerMovement_.z + fMove.z };

    //最大移動スピード・今のスピード
    const float maxMoveSpeed = 1.0f;
    float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerMovement_)));

    if (currentSpeed > maxMoveSpeed) {
        XMVECTOR vMove;
        vMove = XMLoadFloat3(&playerMovement_);
        vMove = XMVector3Normalize(vMove);
        vMove *= maxMoveSpeed;

        XMStoreFloat3(&playerMovement_, vMove);
    }

}
