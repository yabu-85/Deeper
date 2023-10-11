#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Stage.h"

namespace {
    const float defaultPerspectiveDistance_ = 7.0f;
    const float heightRay = 0.5f; //StageとのRay判定の当たった距離にプラスする値

}

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

    //プレイヤーの位置をカメラの焦点とする
    plaPos_ = pPlayer_->GetPosition();

    //マイナスで左、プラスで右寄りになる
    //これの値は多分だけど、画面上の中心から何割先までって感じだと思う：プレイヤーとの距離が近いと値も小さくなる、遠いと大きく
    float cameraOffset = 0.0f;

    cameraTarget_.x = plaPos_.x + (aimDirection_.z * cameraOffset);
    cameraTarget_.y = plaPos_.y + heightDistance_;
    cameraTarget_.z = plaPos_.z - (aimDirection_.x * cameraOffset);

    //カメラ焦点
    XMVECTOR caTarget = XMLoadFloat3(&cameraTarget_);

    RayCastStage();

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

void Aim::RayCastStage()
{
    Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクトを探す
    int hGroundModel = pStage->GetModelHandle();    //モデル番号を取得

    RayCastData data;
    XMFLOAT3 start = pPlayer_->GetPosition();
    start.y += heightDistance_;
    XMFLOAT3 target = XMFLOAT3(cameraPos_.x - start.x, cameraPos_.y - start.y, cameraPos_.z - start.z);
    XMVECTOR vTarget = XMLoadFloat3(&target);
    vTarget = XMVector3Normalize(vTarget);
    XMStoreFloat3(&target, vTarget);

    data.start = start;
    data.dir = target;
    Model::RayCast(hGroundModel, &data); //レイを発射

    //レイが当たったら
    if (data.hit && data.dist < (defaultPerspectiveDistance_ + heightRay))
    {
        float dist = data.dist;
        dist -= heightRay;
        perspectiveDistance_ = dist;

    }
    else {
        perspectiveDistance_ = defaultPerspectiveDistance_;
    }

    return;
}
