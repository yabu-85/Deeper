#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"

namespace {
    float gradually = 0.1f;                 //移動スピードの加減の値
    const float stopGradually = 0.3f;       //移動スピードの加減の値止まるとき
    const float moveGradually = 0.3f;       //移動スピードの加減の値移動時
    const float maxMoveSpeed = 1.0f;        //最大移動スピード

}

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

    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;

}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void Player::Release()
{
}

bool Player::GetMoveVec(XMVECTOR& vec)
{
    vec = XMLoadFloat3(&moveVec_);
    return XMVector3NotEqual(vec, XMVectorZero());
}

void Player::CalcMove()
{
    XMFLOAT3 fMove = { 0,0,0 };
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();
    moveVec_ = { 0,0,0 };
    gradually = stopGradually;

    // PlayerクラスのMove関数内の一部
    if (Input::IsKey(DIK_W)) {
        fMove.x += aimDirection.x;
        fMove.z += aimDirection.z;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_A)) {
        fMove.x -= aimDirection.z;
        fMove.z += aimDirection.x;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_S)) {
        fMove.x -= aimDirection.x;
        fMove.z -= aimDirection.z;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_D)) {
        fMove.x += aimDirection.z;
        fMove.z -= aimDirection.x;
        gradually = moveGradually;
    }

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);
    moveVec_ = fMove;

    fMove = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + fMove.x , 0.0f , playerMovement_.z + fMove.z };

    float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerMovement_)));

    //MaxSpeed超えていたら正規化・MaxSpeedの値にする
    if (currentSpeed > maxMoveSpeed) {
        XMVECTOR vMove;
        vMove = XMLoadFloat3(&playerMovement_);
        vMove = XMVector3Normalize(vMove);
        vMove *= maxMoveSpeed;

        XMStoreFloat3(&playerMovement_, vMove);
    }

}
