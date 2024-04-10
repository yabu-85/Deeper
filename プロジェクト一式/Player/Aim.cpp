#include "Aim.h"
#include "Player.h"
#include "../InputManager.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Global.h"
#include "../Engine/Image.h"
#include "../Stage/CollisionMap.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/Enemymanager.h"
#include "../GameManager/GameManager.h"
#include <vector>

namespace {
    static const float UP_MOUSE_LIMIT = -60.0f;
    static const float DOWN_MOUSE_LIMIT = 60.0f;

    static const int TARGET_CHANGE_COOLTIME = 10;                   //TargetChangeのクールダウン
    static const float TARGET_CHANGE_VALUE = 40.0f;                 //どのくらいマウス動かせばいいか

    static const int COMPULSION_TIME_DEFAULT = 60;                  //強制から戻る時間
    static const float MOUSE_SPEED = 0.05f;                         //感度
    static const float HEIGHT_RAY = 0.1f;                           //RayCastの値にプラスする高さ
    float HEIGHT_DISTANCE = 1.5f;                      //Aimの高さ

    static const float MAX_CAMERA_OFFSET = 5.0f;                    //cameraOffsetの最大距離
    static const float SUPRESS = 0.002f;                            //AimMove時のOffsetの値を抑制する値
    static const float MOVE_SUPRESS = 0.20f;                        //動く時の抑制の値
    static const float STOP_SUPRESS = 0.25f;                        //止まる時の抑制の値
    
    static const float TARGET_RANGE = 25.0f;                        //ターゲットの有効範囲
    static const float FOV_RADIAN = XMConvertToRadians(120) / 2.0f; //ターゲットの有効範囲
    static const float TARGET_RATIO = 0.2f;                         //ターゲット時の回転率
}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPosition_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 }, cameraOffset_{ 0,0,0 },
    compulsionTarget_{ 0,0,0 }, compulsionPosisiton_{ 0,0,0 }, pPlayer_(nullptr), pEnemyBase_(nullptr), pCollisionMap_(nullptr),
    isMove_(true), isCompulsion_(false), isTarget_(false), compulsionTime_(0), iterations_(0), sign_(1), range_(0), moveDistance_(0),
    distanceDecrease_(0), center_{ 0,0,0,0 }, shakeSpeed_(0), rangeDecrease_(0), isTargetChange_(false), targetChangeTime_(0), hPict_(-1),
    shakeDirection_{ 1,0,0,0 }, isValid_(true)
{
    mouseSensitivity = 2.0f;
    defPerspectDistance_ = 5.0f;
    perspectiveDistance_ = defPerspectDistance_;
}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    pPlayer_ = static_cast<Player*>(FindObject("Player"));
    DefaultAim();

    hPict_ = Image::Load("Image/AimTarget.png");
    assert(hPict_ >= 0);
    Transform foundTrans;
    foundTrans.position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    foundTrans.scale_ = XMFLOAT3(0.2f, 0.2f, 0.0f);
    Image::SetAlpha(hPict_, 255);
    Image::SetTransform(hPict_, foundTrans);

}

void Aim::Update()
{
    if (!IsValid()) return;

    //デバッグ用
    if (Input::IsKey(DIK_1)) HEIGHT_DISTANCE += 0.1f;
    if (Input::IsKey(DIK_2)) HEIGHT_DISTANCE -= 0.1f;
    if (Input::IsKey(DIK_3)) defPerspectDistance_ += 0.1f;
    if (Input::IsKey(DIK_4)) defPerspectDistance_ -= 0.1f;

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
        //Target状態の移動・強制から戻る状態だったらTarget状態に移動
        if (isTarget_) {
            CalcCameraOffset(0.0f);
            compulsionTime_ = 0;

            //ちょっとAimTarget時の描画してみる
            XMFLOAT3 tarPos = pEnemyBase_->GetPosition();
            tarPos.y += pEnemyBase_->GetAimTargetPos();
            
            XMFLOAT3 pos = Camera::CalcScreenPosition(tarPos);
            Transform foundTrans;
            foundTrans.position_ = XMFLOAT3(pos.x, pos.y, 0.0f);
            foundTrans.scale_ = XMFLOAT3(0.2f, 0.2f, 0.0f);
            Image::SetAlpha(hPict_, 255);
            Image::SetTransform(hPict_, foundTrans);

            //TargetChange
            XMFLOAT3 mouse = Input::GetMouseMove();
            if (abs(mouse.x) > TARGET_CHANGE_VALUE) {
                if (targetChangeTime_ <= 0 && !isTargetChange_) ChangeTarget(mouse);
            }
            else {
                isTargetChange_ = false;
            }
            targetChangeTime_--;

            FacingTarget();
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
    Image::Draw(hPict_);
    Image::SetAlpha(hPict_, 0);

}

void Aim::Release()
{
}

void Aim::TargetIsDead(EnemyBase* target)
{
    //Aimtargetが引数と同じならTarget状態から外し、新たにTargetがいるか計算
    if (pEnemyBase_ == target) {
        isTarget_ = false;
        SetTargetEnemy();
    }
}

void Aim::SetTargetEnemy()
{
    //すでにターゲット状態の場合はターゲット解除してreturn
    if (isTarget_) {
        isTarget_ = false;
        pEnemyBase_ = nullptr;
        return;
    }

    EnemyBase* t = CalcTargetEnemy();
    if (!t) return;
    isTarget_ = true;
    pEnemyBase_ = t;

}

EnemyBase* Aim::CalcTargetEnemy()
{
    EnemyManager* pEnemyManager = GameManager::GetEnemyManager();
    if (!pEnemyManager) return nullptr;
    std::vector<EnemyBase*> eList = pEnemyManager->GetAllEnemy();

    // プレイヤーの視線方向を計算
    XMFLOAT3 forward;
    forward.x = (float)sin(XMConvertToRadians(transform_.rotate_.y));
    forward.y = 0.0f;
    forward.z = (float)cos(XMConvertToRadians(transform_.rotate_.y));
    XMVECTOR vForward = XMLoadFloat3(&forward);

    float minLeng = 999999;
    int minLengIndex = -1;

    for (int i = 0; i < eList.size(); i++) {
        if(!eList.at(i)->IsAimTarget()) continue;

        //距離計算して、範囲外だったら次
        float dist = DistanceCalculation(cameraTarget_, eList.at(i)->GetPosition());
        if (dist >= TARGET_RANGE) continue;

        //視野角を計算
        XMFLOAT3 toTarget = Float3Sub(cameraTarget_, eList.at(i)->GetPosition());
        float dot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(XMLoadFloat3(&toTarget)), vForward));
        float angle = acosf(dot);

        //ターゲットが範囲内にあるかどうかを確認
        if (angle <= FOV_RADIAN) {
            if (minLeng > dist) {
                minLeng = dist;
                minLengIndex = i;
            }
        }
    }

    if (minLengIndex >= 0) return eList.at(minLengIndex);
    return nullptr;

}

void Aim::SetCameraShake(const CameraShakeInfo& info)
{
    iterations_ = info.iterat;
    range_ = info.range;
    moveDistance_ = info.range;
    shakeSpeed_ = info.speed;
    distanceDecrease_ = info.speed_decrease;
    rangeDecrease_ = info.range_decrease;
    
    center_ = XMVectorZero();
    sign_ = 1.0f;

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

    CameraShake();

    //RayCastの前に情報を入れる
    perspectiveDistance_ = perspectiveDistance_ + ((defPerspectDistance_ - perspectiveDistance_) * 0.1f);
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

    //範囲外に行った場合の処理をやる
    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&targetPos) - XMLoadFloat3(&plaPos) )) > TARGET_RANGE) {
        isTarget_ = false;
        pEnemyBase_ = nullptr;
        return;
    }

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

void Aim::ChangeTarget(XMFLOAT3 mouse)
{
    EnemyManager* pEnemyManager = GameManager::GetEnemyManager();
    if (!pEnemyManager) return;
    std::vector<EnemyBase*> eList = pEnemyManager->GetAllEnemy();
    if (eList.empty()) return;

    // プレイヤーの視線方向を計算
    XMFLOAT3 playerForward;
    playerForward.x = (float)sin(XMConvertToRadians(transform_.rotate_.y));
    playerForward.y = 0.0f;
    playerForward.z = (float)cos(XMConvertToRadians(transform_.rotate_.y));
    XMVECTOR vPlayerForward = XMLoadFloat3(&playerForward);
    XMFLOAT3 pPos = pPlayer_->GetPosition();

    int rIndex = -1;
    int lIndex = -1;
    float minR = 999999.0f;
    float minL = -999999.0f;
    for (int i = 0; i < eList.size(); i++) {
        if (pEnemyBase_ == eList.at(i)) continue;

        XMFLOAT3 ePos = eList.at(i)->GetPosition();
        XMVECTOR vVec = XMLoadFloat3(&pPos) - XMLoadFloat3(&ePos);
        XMFLOAT3 vec = XMFLOAT3();
        XMStoreFloat3(&vec, vVec);

        //範囲外だったら次
        float distance = sqrtf(vec.x * vec.x + vec.z * vec.z);
        if (distance >= TARGET_RANGE) continue;

        //視野角を計算
        float dotProduct = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vVec), vPlayerForward));
        float angle = acosf(dotProduct);
        if (angle > FOV_RADIAN) continue;

        //視線上に合るから・右か左か計算
        float dot = XMVectorGetY(XMVector3Cross(vPlayerForward, vVec));
        float fovRa = FOV_RADIAN;
        //右にある
        if (dot > 0.0f && dot < minR) {
            minR = dot;
            rIndex = i;
        }
        //左にある（視線上の場合も
        else if (dot < 0.0f && dot > minL) {
            minL = dot;
            lIndex = i;
        }
    }

    //右を選ぶ
    if (mouse.x > 0.0f) {
        if (rIndex != -1) {
            pEnemyBase_ = eList.at(rIndex);
            isTargetChange_ = true;
            targetChangeTime_ = TARGET_CHANGE_COOLTIME;
        }
    }
    //左を選ぶ
    else if (lIndex != -1) {
        pEnemyBase_ = eList.at(lIndex);
        isTargetChange_ = true;
        targetChangeTime_ = TARGET_CHANGE_COOLTIME;
    }
}

void Aim::CalcCameraOffset(float _aimMove)
{
    //マウスの移動量で offsetの値を抑制
    cameraOffset_.x += (cameraOffset_.x * -1.0f) * abs(_aimMove);
    cameraOffset_.z += (cameraOffset_.z * -1.0f) * abs(_aimMove);

    XMVECTOR vCameraOffset = XMLoadFloat3(&cameraOffset_);
    XMVECTOR vTargetOffset = pPlayer_->GetMovementVector();
    vTargetOffset *= MAX_CAMERA_OFFSET * -1;

    if (InputManager::CmdWalk()) vCameraOffset += (vTargetOffset - vCameraOffset) * MOVE_SUPRESS;   //move
    else vCameraOffset += (vTargetOffset - vCameraOffset) * STOP_SUPRESS;                     //stop

    XMStoreFloat3(&cameraOffset_, vCameraOffset);
}

void Aim::RayCastStage()
{
    pCollisionMap_ = GameManager::GetCollisionMap();
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
    pCollisionMap_->GetRayCastMinDist(cameraPosition_, cameraTarget_, &data, min);

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

void Aim::CameraShake()
{
    if (iterations_ <= 0) return;

    //カメラシェイク終了戻る処理
    if (iterations_ == 1) {
        float dist = XMVectorGetX(XMVector3Length(center_));
        XMVECTOR vec = -XMVector3Normalize(center_);

        //目標地点につくよ
        if (dist < moveDistance_ * shakeSpeed_) {
            moveDistance_ = dist;
            iterations_--;
            center_ = XMVectorZero();
            sign_ = 1.0f;
        }

        vec *= moveDistance_ * shakeSpeed_;
        center_ += vec;

        cameraTarget_.x += center_.m128_f32[0];
        cameraTarget_.y += center_.m128_f32[1];
        cameraTarget_.z += center_.m128_f32[2];
        return;
    }

    //移動方向・移動目標場所を計算
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMVECTOR shakeDirection = XMVector3Normalize(XMVector3TransformNormal((shakeDirection_), mRotY));
    XMVECTOR nextPosition = center_ + shakeDirection;

    //方向調べて移動させる
    XMVECTOR directionVector = nextPosition - center_;
    directionVector = XMVector3NormalizeEst(directionVector) * moveDistance_ * shakeSpeed_;
    center_ += directionVector;

    //同じ方向か調べる
    bool sign2 = (center_.m128_f32[0] + center_.m128_f32[1] + center_.m128_f32[2] / 3) > 0.0f;
    bool sign3 = (directionVector.m128_f32[0] + directionVector.m128_f32[1] + directionVector.m128_f32[2] / 3) > 0;

    //目標地点を超えたか調べる
    float cen = XMVectorGetX(XMVector3Length(center_));
    if (sign2 == sign3 && cen > range_) {
        iterations_--;
        sign_ *= -1.0f;
        shakeDirection_ *= -1.0f;
        center_ = XMVector3Normalize(center_) * range_;
       
        moveDistance_ *= distanceDecrease_;
        range_ *= rangeDecrease_;
    }

    cameraTarget_.x += center_.m128_f32[0];
    cameraTarget_.y += center_.m128_f32[1];
    cameraTarget_.z += center_.m128_f32[2];
}
