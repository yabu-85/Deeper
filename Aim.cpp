#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Player.h"

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 },
    plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimMove_(false)
{
    mouseSensitivity = 2.0f;
    perspectiveDistance_ = 7.0f;
    heightDistance_ = 1.5f;
}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    pPlayer_ = (Player*)FindObject("Player");

    aimMove_ = true;
}

void Aim::Update()
{
    if (!aimMove_) return;

    XMFLOAT3 mouseMove = Input::GetMouseMove(); //マウスの移動量を取得

    //移動量を計算
    transform_.rotate_.y += (mouseMove.x * 0.05f) * mouseSensitivity; //横方向の回転
    transform_.rotate_.x -= (mouseMove.y * 0.05f) * mouseSensitivity; //縦方向の回転
    if (transform_.rotate_.x <= -89.0f) transform_.rotate_.x = -89.0f;
    if (transform_.rotate_.x >= 89.0f) transform_.rotate_.x = 89.0f;

    //カメラの回転
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    //カメラの位置と回転を合成
    XMMATRIX mView = mRotX * mRotY;

    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR camPos = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormalを使用することで回転のみを適用します
    XMVector3Normalize(camPos);
    XMStoreFloat3(&aimDirection_, -camPos);

    //マイナスで左、プラスで右寄りになる
    //これの値は多分だけど、画面上の中心から何割先までって感じだと思う：プレイヤーとの距離が近いと値も小さくなる、遠いと大きく
    float cameraOffset = 0.0f;

    //プレイヤーの位置をカメラの焦点とする
    plaPos_ = pPlayer_->GetPosition();
    cameraPos_.x = plaPos_.x + (aimDirection_.z * cameraOffset);
    cameraPos_.y = plaPos_.y + heightDistance_;
    cameraPos_.z = plaPos_.z - (aimDirection_.x * cameraOffset);

    //カメラ焦点
    XMVECTOR caTarget = XMLoadFloat3(&cameraPos_);

    //プレイヤーの半径を考慮して回転を適用している
    //ここAimの近さの値をプレイヤーから取得して計算もしてる
    camPos = caTarget + (camPos * perspectiveDistance_);

    XMStoreFloat3(&cameraPos_, camPos);
    XMStoreFloat3(&cameraTarget_, caTarget);

    Camera::SetPosition(cameraPos_);
    Camera::SetTarget(cameraTarget_);

}

void Aim::Draw()
{
}

void Aim::Release()
{
}
