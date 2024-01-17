#include "Aim.h"
#include "Engine/Camera.h"
#include "Player.h"
#include "CollisionMap.h"
#include <vector>
#include "EnemyBase.h"
#include "Enemymanager.h"
#include "GameManager.h"
#include "PlayerCommand.h"

//デバッグ用
#include "Engine/Input.h"

namespace {
    static const int COMPULSION_TIME_DEFAULT = 60;

    static const float HEIGHT_DISTANCE = 3.0f;
    static const float UP_MOUSE_LIMIT = -60.0f;
    static const float DOWN_MOUSE_LIMIT = 60.0f;
    static const float MOUSE_SPEED = 0.05f;
    static const float HEIGHT_RAY = 0.1f;                           //RayCastの値にプラスする高さ
    static const float MAX_CAMERA_OFFSET = 2.0f;                    //cameraOffsetの最大距離
    static const float SUPRESS = 0.002f;                            //Offsetの値を抑えるやつ
    static const float MOVE_SUPRESS = 0.03f;                        //動く時の抑制の値
    static const float STOP_SUPRESS = 0.06f;                        //止まる時の抑制の値
    static const float TARGET_RANGE = 50.0f;                        //ターゲットの有効範囲
    static const float FOV_RADIAN = XMConvertToRadians(60) / 2.0f;  //ターゲットの有効範囲
    static const float TARGET_RATIO = 0.2f;                         //ターゲット時の回転率

}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPosition_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 }, cameraOffset_{ 0,0,0 },
    compulsionTarget_{ 0,0,0 }, compulsionPosisiton_{ 0,0,0 }, pPlayer_(nullptr), pEnemyBase_(nullptr), pCollisionMap_(nullptr),
    isMove_(true), isCompulsion_(false), isTarget_(false), compulsionTime_(0)
{
    mouseSensitivity = 2.0f;
    defPerspectDistance_ = 10.0f;
    perspectiveDistance_ = defPerspectDistance_;
}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    pPlayer_ = static_cast<Player*>(FindObject("Player"));
    DefaultAim();

}

void Aim::Update()
{
    //デバッグ用
    if (Input::IsKeyDown(DIK_T)) isMove_ = !isMove_;
    if (Input::IsKey(DIK_X)) defPerspectDistance_ += 0.1f;
    if (Input::IsKey(DIK_Z)) defPerspectDistance_ -= 0.1f;

    if (compulsionTime_ > 0) {
        //強制移動
        if (isCompulsion_) {
            Compulsion();
            isCompulsion_ = false;
            return;
        }

        //戻ってる時の移動
        if (!isTarget_) {
            BackCompulsion();
            compulsionTime_--;
            return;
        }
    }

    if (isMove_) {
        if (isTarget_) {
            CalcCameraOffset(0.0f);

            //ターゲットが生きてるならそいつにAim合わせる
            if (!pEnemyBase_->IsDead()) FacingTarget();
            else {
                //死んでたら今向いている方向にターゲットできるEnemyがいるならそいつをターゲットにする
                isTarget_ = false;
                SetTargetEnemy();
            }
        }

        //マウスで視点移動
        else {
            CalcMouseMove();
            CalcCameraOffset(Input::GetMouseMove().x * SUPRESS);
        }
    }
    else {
        CalcCameraOffset(0.0f);
    }

    DefaultAim();
}

void Aim::Draw()
{
}

void Aim::Release()
{
}

void Aim::SetTargetEnemy()
{
    //すでにターゲット状態の場合はターゲット解除してreturn
    if (isTarget_) {
        isTarget_ = false;
        return;
    }

    EnemyManager* pEnemyManager = GameManager::GetEnemyManager();
    std::vector<EnemyBase*> eList = pEnemyManager->GetAllEnemy();

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
        if (distance >= TARGET_RANGE) continue;

        // 方向ベクトルを正規化
        XMVECTOR toTargetNorm = XMVector3Normalize(XMLoadFloat3(&toTarget));
        XMVECTOR playerForwardNorm = XMLoadFloat3(&playerForward);

        // 視野角を計算
        float dotProduct = XMVectorGetX(XMVector3Dot(toTargetNorm, playerForwardNorm));
        float angle = acosf(dotProduct);

        // 角度を比較してターゲットが範囲内にあるかどうかを確認
        if (angle <= FOV_RADIAN) {
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

void Aim::SetCompulsion(XMFLOAT3 pos, XMFLOAT3 tar)
{
    compulsionPosisiton_ = pos;
    compulsionTarget_ = tar;
    isCompulsion_ = true;
    compulsionTime_ = COMPULSION_TIME_DEFAULT;

}

//------------------------------------private--------------------------------------------

void Aim::DefaultAim()
{
    XMVECTOR direction = CalcDirection(transform_.rotate_.x, transform_.rotate_.y);
    XMStoreFloat3(&aimDirection_, -direction);

    //プレイヤーの位置をカメラの焦点とする        
    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    cameraTarget_ = { plaPos.x + cameraOffset_.x, plaPos.y + HEIGHT_DISTANCE, plaPos.z + cameraOffset_.z };

    //RayCastの前に情報を入れる
    XMVECTOR camPos = XMLoadFloat3(&cameraTarget_) + (direction * perspectiveDistance_);
    XMStoreFloat3(&cameraPosition_, camPos);

    //RayCastしてその値を上書きする
    RayCastStage();

    //カメラ情報をセット
    Camera::SetPosition(cameraPosition_);
    Camera::SetTarget(cameraTarget_);
}

void Aim::Compulsion()
{
    XMStoreFloat3(&cameraPosition_, (XMVectorLerp(XMLoadFloat3(&cameraPosition_), XMLoadFloat3(&compulsionPosisiton_), 0.05f)));
    XMStoreFloat3(&cameraTarget_, (XMVectorLerp(XMLoadFloat3(&cameraTarget_), XMLoadFloat3(&compulsionTarget_), 0.05f)));
    
    XMVECTOR dir = XMLoadFloat3(&cameraPosition_) - XMLoadFloat3(&cameraTarget_);
    perspectiveDistance_ = XMVectorGetX(XMVector3Length(dir));
    RayCastStage();

    Camera::SetPosition(cameraPosition_);
    Camera::SetTarget(cameraTarget_);

    //強制移動時のRotateを求める
    dir = XMVector3Normalize(dir);
    float rotationY = atan2f(XMVectorGetX(dir), XMVectorGetZ(dir));
    float rotationX = -asinf(XMVectorGetY(dir));
    transform_.rotate_.x = compulsionPosisiton_.x = XMConvertToDegrees(rotationX);
    transform_.rotate_.y = compulsionPosisiton_.y = XMConvertToDegrees(rotationY);

    //Rotateの結果を使ってAimDirectionを計算してセット
    XMVECTOR direction = CalcDirection(transform_.rotate_.x, transform_.rotate_.y);
    XMStoreFloat3(&aimDirection_, -direction);
}

void Aim::BackCompulsion()
{
    CalcCameraOffset(0.0f);
    CalcMouseMove();

    //強制時の視点の距離を求める
    XMVECTOR dir = XMLoadFloat3(&cameraPosition_) - XMLoadFloat3(&cameraTarget_);
    float dist = XMVectorGetX(XMVector3Length(dir));
    perspectiveDistance_ = perspectiveDistance_ - ((dist - defPerspectDistance_) / (float)compulsionTime_);

    //戻り中のPositionとTargetを計算する
    XMFLOAT3 position = pPlayer_->GetPosition();
    XMFLOAT3 target = { position.x + cameraOffset_.x, position.y + HEIGHT_DISTANCE, position.z + cameraOffset_.z };
    XMVECTOR camPos = XMLoadFloat3(&target) + (CalcDirection(transform_.rotate_.x, transform_.rotate_.y) * perspectiveDistance_);
    XMStoreFloat3(&position, camPos);

    XMVECTOR vPos = XMLoadFloat3(&cameraPosition_);
    vPos = vPos - ((vPos - XMLoadFloat3(&position)) / (float)compulsionTime_);
    XMStoreFloat3(&cameraPosition_, vPos);

    XMVECTOR vTar = XMLoadFloat3(&cameraTarget_);
    vTar = vTar - ((vTar - XMLoadFloat3(&target)) / (float)compulsionTime_);
    XMStoreFloat3(&cameraTarget_, vTar);

    //計算結果を使ってRayCastするそれをセット
    RayCastStage();
    Camera::SetPosition(cameraPosition_);
    Camera::SetTarget(cameraTarget_);

    //強制移動時のRotate・Directionを求める
    dir = XMVector3Normalize(dir);
    float rotationY = atan2f(XMVectorGetX(dir), XMVectorGetZ(dir));
    float rotationX = -asinf(XMVectorGetY(dir));
    transform_.rotate_.x = XMConvertToDegrees(rotationX);
    transform_.rotate_.y = XMConvertToDegrees(rotationY);
    XMVECTOR direction = CalcDirection(transform_.rotate_.x, transform_.rotate_.y);
    XMStoreFloat3(&aimDirection_, -direction);
}

void Aim::FacingTarget()
{
    //プレイヤーの方向に向くようにする
    XMVECTOR vFront{ 0,0,1,0 };
    float aimTarPos = pEnemyBase_->GetAimTargetPos();
    XMFLOAT3 targetPos = pEnemyBase_->GetPosition();
    targetPos = XMFLOAT3(targetPos.x, targetPos.y + aimTarPos, targetPos.z);

    XMFLOAT3 fAimPos = XMFLOAT3(cameraPosition_.x - targetPos.x, 0.0f, cameraPosition_.z - targetPos.z);
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

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMFLOAT2 b = XMFLOAT2(sinf(angle), cosf(angle));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * TARGET_RATIO);

    //-----------------------------X軸計算-----------------------------
    // カメラからターゲットへの方向ベクトルを計算
    XMFLOAT3 taTarget = XMFLOAT3(targetPos.x, 0.0f, targetPos.z);
    XMFLOAT3 taCamPos = XMFLOAT3(cameraPosition_.x, 0.0f, cameraPosition_.z);
    XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&taTarget), XMLoadFloat3(&taCamPos));

    float distance = XMVectorGetX(XMVector3Length(direction));
    float height = cameraPosition_.y - targetPos.y;

    // 距離と高さに基づいてX軸回転角度を計算
    float rotationX = (float)atan2(height, distance);

    a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.x)), cosf(XMConvertToRadians(transform_.rotate_.x)));
    b = XMFLOAT2(sinf(-rotationX), cosf(-rotationX));
    vA = XMVector2Normalize(XMLoadFloat2(&a));
    vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    cross = a.x * b.y - a.y * b.x;
    dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.x += XMConvertToDegrees(-atan2f(cross, dot) * TARGET_RATIO);

    if (transform_.rotate_.x <= UP_MOUSE_LIMIT) transform_.rotate_.x = UP_MOUSE_LIMIT;
    if (transform_.rotate_.x >= DOWN_MOUSE_LIMIT) transform_.rotate_.x = DOWN_MOUSE_LIMIT;

}

void Aim::CalcCameraOffset(float _aimMove)
{
    //マウスの移動量で offsetの値を抑制
    cameraOffset_.x += (cameraOffset_.x * -1.0f) * abs(_aimMove);
    cameraOffset_.z += (cameraOffset_.z * -1.0f) * abs(_aimMove);

    XMVECTOR vCameraOffset = XMLoadFloat3(&cameraOffset_);
    XMVECTOR vTargetOffset = pPlayer_->GetMovement();
    vTargetOffset *= MAX_CAMERA_OFFSET * -1;

    if (pPlayer_->GetCommand()->CmdWalk()) vCameraOffset += (vTargetOffset - vCameraOffset) * MOVE_SUPRESS;   //move
    else vCameraOffset += (vTargetOffset - vCameraOffset) * STOP_SUPRESS;                     //stop

    XMStoreFloat3(&cameraOffset_, vCameraOffset);
}

void Aim::RayCastStage()
{
    pCollisionMap_ = static_cast<CollisionMap*>(FindObject("CollisionMap"));
    if (pCollisionMap_ == nullptr) return;

    RayCastData data;
    XMFLOAT3 start = cameraTarget_;
    XMVECTOR vDir = XMLoadFloat3(&cameraPosition_) - XMLoadFloat3(&cameraTarget_);
    vDir = XMVector3Normalize(vDir);
    XMFLOAT3 dir = XMFLOAT3();
    XMStoreFloat3(&dir, vDir);
    data.start = start;
    data.dir = dir;
    float min = 0.0f;
    pCollisionMap_->GetRayCastMinDist(cameraPosition_, pPlayer_->GetPosition(), &data, min);

    //レイ当たった・判定距離内だったら
    if (min <= (defPerspectDistance_)) {
        perspectiveDistance_ = min - HEIGHT_RAY;
    }

    XMVECTOR camPos = XMLoadFloat3(&cameraTarget_) + (vDir * perspectiveDistance_);
    XMStoreFloat3(&cameraPosition_, camPos);

}

void Aim::CalcMouseMove()
{
    XMFLOAT3 mouseMove = Input::GetMouseMove(); //マウスの移動量を取得
    transform_.rotate_.y += (mouseMove.x * MOUSE_SPEED) * mouseSensitivity; //横方向の回転
    transform_.rotate_.x -= (mouseMove.y * MOUSE_SPEED) * mouseSensitivity; //縦方向の回転
    if (transform_.rotate_.x <= UP_MOUSE_LIMIT) transform_.rotate_.x = UP_MOUSE_LIMIT;
    if (transform_.rotate_.x >= DOWN_MOUSE_LIMIT) transform_.rotate_.x = DOWN_MOUSE_LIMIT;

}

XMVECTOR Aim::CalcDirection(float x, float y)
{
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(y));
    XMMATRIX mView = mRotX * mRotY;
    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR direction = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormalを使用することで回転のみを適用します
    return XMVector3Normalize(direction);
}
