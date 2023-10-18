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
#include "EnemySpawnCtrl.h"
#include "GameManager.h"

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

    const float targetRange = 30.0f;        //ターゲットの有効範囲
    const float fovRadian = XMConvertToRadians(60) / 2;
}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 },
    plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimMove_(false), cameraOffset_{0,0,0}, isTarget_(false), pEnemyBase_(nullptr)
{
    mouseSensitivity = 2.0f;
    perspectiveDistance_ = defPerspectDistance;
    heightDistance_ = 2.0f; //近いとだめ
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

    if (isTarget_) {
        //カメラオフセットの機能はターゲット時も有効にする、ただしマウスでの抑制はなし
        CalcCameraOffset(0.0f);

        //isTarget状態なのにTargetとなるEnemyがいない場合reset
        if (!pEnemyBase_->IsDead()) FacingTarget();
        else isTarget_ = false;
        
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

void Aim::SetTargetEnemy()
{
    //ターゲット解除の場合はreturn
    if (isTarget_) {
        isTarget_ = false;
        return;
    }

    GameManager* pGM = (GameManager*)FindObject("GameManager");
    EnemySpawnCtrl* pEnemySpawnCtrl = pGM->GetEnemySpawnCtrl();
    std::vector<EnemyBase*> eList = pEnemySpawnCtrl->GetAllEnemy();

    // プレイヤーの視線方向を計算
    XMFLOAT3 playerForward;
    playerForward.x = (float)sin(XMConvertToRadians(transform_.rotate_.y));
    playerForward.y = 0.0f;
    playerForward.z = (float)cos(XMConvertToRadians(transform_.rotate_.y));

    float minLeng = 999999;
    int minLengIndex = -1;

    for (int i = 0; i < eList.size(); i++) {
        XMFLOAT3 ePos = eList.at(i)->GetPosition();
        
        //ターゲットへのベクトルを計算（逆ベクトル）
        XMFLOAT3 toTarget = XMFLOAT3(cameraTarget_.x - ePos.x, 0.0f, cameraTarget_.z - ePos.z);

        //ターゲットへの距離を計算
        float distance = (float)sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z);

        //範囲外だったら次
        if (distance >= targetRange) continue;

        // 方向ベクトルを正規化
        XMVECTOR toTargetNorm = XMVector3Normalize(XMLoadFloat3(&toTarget));
        XMVECTOR playerForwardNorm = XMLoadFloat3(&playerForward);

        // 視野角を計算
        float dotProduct = XMVectorGetX(XMVector3Dot(toTargetNorm, playerForwardNorm));
        float angle = acosf(dotProduct);

        // 角度を比較してターゲットが範囲内にあるかどうかを確認
        if (angle <= fovRadian) {
            if (minLeng > distance) {
                minLeng = distance;
                minLengIndex = i;
            }
        }
    }

    if (minLengIndex >= 0) {
        pEnemyBase_ = eList.at(minLengIndex);
        isTarget_ = true;
    }

}

//-----------private--------------------------------------------

void Aim::FacingTarget()
{
    //プレイヤーの方向に向くようにする
    XMVECTOR vFront{ 0,0,1,0 };
    XMFLOAT3 targetPos = pEnemyBase_->GetPosition();

    XMFLOAT3 fAimPos = XMFLOAT3(cameraPos_.x - targetPos.x, 0.0f, cameraPos_.z - targetPos.z);
    XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);  //正規化用の変数にfloatの値を入れる
    vAimPos = XMVector3Normalize(vAimPos);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos);
    float dot = XMVectorGetX(vDot);
    float angle = (float)acos(dot);

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
    float rotationX = (float)atan2(height, distance);

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
    if (pCollisionMap == nullptr) return;

    int dataSize = 0;
    std::vector<Triangle*> datas = pCollisionMap->GetCellInTriangle();
    dataSize = (int)datas.size();

    RayCastData data;
    XMFLOAT3 start = _start;
    XMFLOAT3 dir = XMFLOAT3(cameraPos_.x - start.x, cameraPos_.y - start.y, cameraPos_.z - start.z);
    XMVECTOR vDir = XMLoadFloat3(&dir);
    vDir = XMVector3Normalize(vDir);
    XMStoreFloat3(&dir, vDir);
    
    const float minRangeMax = 100000000;
    float minRange = minRangeMax;
    for (int i = 0; i < dataSize; i++) {
        data.start = start;
        data.dir = dir;

        Triangle tri = *datas.at(i);
        tri.RayCast(&data, tri);

        //レイ当たった・判定距離内だったら
        if (data.hit && data.dist < (defPerspectDistance + heightRay))
        {
            //最小より小さければ上書き
            float range = data.dist - heightRay;
            if(minRange > range) minRange = range;
        }
    }

    //当たったらMinRange、当たらなければデフォルト
    if (minRange < minRangeMax) perspectiveDistance_ = minRange;
    else perspectiveDistance_ = defPerspectDistance;

    return;
}