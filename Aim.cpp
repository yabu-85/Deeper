#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"
#include "Triangle.h"
#include <vector>
#include "EnemyBase.h"

namespace {
    const float upMouselimit = -89.0f;
    const float donwMouselimit = 70.0f;
    const float mouseSpeed = 0.05f;

    const float defPerspectDistance = 8.0f; //デフォルトの視点の距離
    const float heightRay = 0.5f;           //StageとのRay判定の当たった距離にプラスする値

    const float numSupress = 0.002f;        //マウス移動でOffsetの値を戻す量
    const float maxCameraOffset = 2.0f;     //cameraOffsetの最大距離
    const float moveAimTime = 0.05f;        //動く時の抑制の値
    const float stopAimTime = 0.05f;        //止まる時の抑制の値

}

float easeOutQuad(float number) {
    return 1.0f - (1.0f - number) * (1.0f - number);
}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 },
    plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimMove_(false), cameraOffset_{0,0,0}, isTarget_(false)
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
    if (Input::IsMouseButtonDown(1)) aimMove_ = !aimMove_;

    if (!aimMove_) return;
    if (Input::IsKeyDown(DIK_Q)) isTarget_ = !isTarget_;

    if (isTarget_) {
        //カメラオフセットの機能はターゲット時も有効にする、ただしマウスでの抑制はなし
        CalcCameraOffset(0.0f);
        FacingTarget();
        
    }
    else {
        XMFLOAT3 mouseMove = Input::GetMouseMove(); //マウスの移動量を取得
        transform_.rotate_.y += (mouseMove.x * mouseSpeed) * mouseSensitivity; //横方向の回転
        transform_.rotate_.x -= (mouseMove.y * mouseSpeed) * mouseSensitivity; //縦方向の回転
        if (transform_.rotate_.x <= upMouselimit) transform_.rotate_.x = upMouselimit;
        if (transform_.rotate_.x >= donwMouselimit) transform_.rotate_.x = donwMouselimit;

        CalcCameraOffset(mouseMove.x * numSupress);
    }

    //カメラの回転
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    //カメラの位置と回転を合成
    XMMATRIX mView = mRotX * mRotY;

    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR direction = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormalを使用することで回転のみを適用します
    XMVector3Normalize(direction);
    XMStoreFloat3(&aimDirection_, -direction);

    //プレイヤーの位置をカメラの焦点とする
    plaPos_ = pPlayer_->GetPosition();

    cameraTarget_.x = plaPos_.x + cameraOffset_.x;
    cameraTarget_.y = plaPos_.y + heightDistance_;
    cameraTarget_.z = plaPos_.z + cameraOffset_.z;

    RayCastStage(cameraTarget_);

    //カメラ焦点
    XMVECTOR caTarget = XMLoadFloat3(&cameraTarget_);

    //ここAimの近さの値をプレイヤーから取得して計算もしてる
    XMVECTOR camPos = caTarget + (direction * perspectiveDistance_);

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

void Aim::FacingTarget()
{
    //プレイヤーの方向に向くようにする
    XMVECTOR vFront{ 0,0,1,0 };
    XMFLOAT3 targetPos = { 20.0f, 0.0f, 0.0f };

    EnemyBase* pEnemyBase = (EnemyBase*)FindObject("EnemyBase");
    targetPos = pEnemyBase->GetPosition();

    XMFLOAT3 fAimPos = XMFLOAT3(cameraPos_.x - targetPos.x, 0.0f, cameraPos_.z - targetPos.z);
    XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);  //正規化用の変数にfloatの値を入れる
    vAimPos = XMVector3Normalize(vAimPos);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos);
    float dot = XMVectorGetX(vDot);
    float angle = acos(dot);

    //外積求めて半回転だったら angle に -1 掛ける
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos); //外積求めるやつ 外積はベクトル型
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }
    transform_.rotate_.y = XMConvertToDegrees(angle);

    //-----------------------------X軸計算-----------------------------
    // カメラからターゲットへの方向ベクトルを計算
    XMFLOAT3 taTarget = XMFLOAT3(targetPos.x, 0.0f, targetPos.z);
    XMFLOAT3 taCamPos = XMFLOAT3(cameraPos_.x, 0.0f, cameraPos_.z);
    XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&taTarget), XMLoadFloat3(&taCamPos));

    // カメラとターゲット間の距離を計算
    float distance = XMVectorGetX(XMVector3Length(direction));

    // カメラとターゲット間の高さ差を計算
    float height = cameraPos_.y - targetPos.y;

    // 距離と高さに基づいてX軸回転角度を計算
    float rotationX = atan2(height, distance);

    // 回転角度を度数に変換
    rotationX = -XMConvertToDegrees(rotationX);

    transform_.rotate_.x = rotationX;
    if (transform_.rotate_.x <= upMouselimit) transform_.rotate_.x = upMouselimit;
    if (transform_.rotate_.x >= donwMouselimit) transform_.rotate_.x = donwMouselimit;

}

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
    CollisionMap* pCollisionMap = (CollisionMap*)FindObject("CollisionMap");
    if (pCollisionMap == nullptr)
        return;

    int dataSize = 0;

    std::vector<Triangle*> datas = pCollisionMap->GetCellInTriangle();
    dataSize = (int)datas.size();

    RayCastData data;
    XMFLOAT3 start = _start;
    XMFLOAT3 dir = XMFLOAT3(cameraPos_.x - start.x, cameraPos_.y - start.y, cameraPos_.z - start.z);
    XMVECTOR vDir = XMLoadFloat3(&dir);
    vDir = XMVector3Normalize(vDir);
    XMStoreFloat3(&dir, vDir);
    
    bool rayHit = false;
    for (int i = 0; i < dataSize; i++) {
        data.start = start;
        data.dir = dir;

        Triangle tri = *datas.at(i);
        tri.RayCast(&data, tri);

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