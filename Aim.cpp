#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Stage.h"
#include <vector>

namespace {
    const float upMouselimit = -89.0f;
    const float donwMouselimit = 70.0f;
    const float mouseSpeed = 0.05f;

    const float defPerspectDistance = 8.0f; //デフォルトの視点の距離
    const float heightRay = 0.5f;           //StageとのRay判定の当たった距離にプラスする値

    const float numSupress = 0.002f;        //マウス移動でOffsetの値を戻す量
    const float maxCameraOffset = 2.0f;     //cameraOffsetの最大距離
    const float moveAimTime = 0.07f;        //動く時の抑制の値
    const float stopAimTime = 0.05f;        //止まる時の抑制の値

}

float easeOutQuad(float number) {
    return 1.0f - (1.0f - number) * (1.0f - number);
}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 },
    plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimMove_(false), cameraOffset_{0,0,0}
{
    mouseSensitivity = 2.0f;
    perspectiveDistance_ = 7.0f;
    heightDistance_ = 2.0f;
}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    pPlayer_ = (Player*)FindObject("Player");
    aimMove_ = true;
	
	transform_.rotate_.y = 180;
}

void Aim::Update()
{
    if (!aimMove_) return;

    XMFLOAT3 mouseMove = Input::GetMouseMove(); //マウスの移動量を取得

    //移動量を計算
    transform_.rotate_.y += (mouseMove.x * mouseSpeed) * mouseSensitivity; //横方向の回転
    transform_.rotate_.x -= (mouseMove.y * mouseSpeed) * mouseSensitivity; //縦方向の回転
    if (transform_.rotate_.x <= upMouselimit) transform_.rotate_.x = upMouselimit;
    if (transform_.rotate_.x >= donwMouselimit) transform_.rotate_.x = donwMouselimit;

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

    CalcCameraOffset(mouseMove.x * numSupress);
    cameraTarget_.x = plaPos_.x + cameraOffset_.x;
    cameraTarget_.y = plaPos_.y + heightDistance_;
    cameraTarget_.z = plaPos_.z + cameraOffset_.z;

    RayCastStage(cameraTarget_);

    //カメラ焦点
    XMVECTOR caTarget = XMLoadFloat3(&cameraTarget_);

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

//-----------private-------------------

void Aim::CalcCameraOffset(float _aimMove)
{
    //マウスの移動量で offsetの値を抑制
    cameraOffset_.x += (cameraOffset_.x * -1.0f) * abs(_aimMove);
    cameraOffset_.z += (cameraOffset_.z * -1.0f) * abs(_aimMove);
    
    XMVECTOR vCameraOffset = XMLoadFloat3(&cameraOffset_);
    XMVECTOR vTargetOffset;

    //移動キーの情報を取得＆方向を逆に
    bool plaMoveInput = pPlayer_->GetMoveVec(vTargetOffset);
    vTargetOffset *= maxCameraOffset * -1;

    if(plaMoveInput) vCameraOffset += (vTargetOffset - vCameraOffset) * moveAimTime;   //move
    else vCameraOffset += (vTargetOffset - vCameraOffset) * stopAimTime;               //stop

    XMStoreFloat3(&cameraOffset_, vCameraOffset);
}

void Aim::RayCastStage(XMFLOAT3 _start)
{
    Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクトを探す
    std::vector<IntersectData> datas = pStage->GetModelHandle();    //モデル番号を取得

    RayCastData data;
    XMFLOAT3 start = _start;
    XMFLOAT3 dir = XMFLOAT3(cameraPos_.x - start.x, cameraPos_.y - start.y, cameraPos_.z - start.z);
    XMVECTOR vDir = XMLoadFloat3(&dir);
    vDir = XMVector3Normalize(vDir);
    XMStoreFloat3(&dir, vDir);

    int arraySize = datas.size();
    bool rayHit = false;

    for (int i = 0; i < arraySize; i++) {
        Transform trans;
        trans.position_ = datas.back().position;
        int hGround = datas.back().hModelNum;
        datas.pop_back();

        data.start = start;
        data.dir = dir;
        Model::SetTransform(hGround, trans);
        Model::RayCast(hGround, &data);

        //レイ当たった・判定距離内だったら
        if (data.hit && data.dist < (defPerspectDistance + heightRay))
        {
            float dist = data.dist;
            dist -= heightRay;
            perspectiveDistance_ = dist;
            rayHit = true;
        }
    }

    if (!rayHit) {
        perspectiveDistance_ = defPerspectDistance;
    }

    return;
}