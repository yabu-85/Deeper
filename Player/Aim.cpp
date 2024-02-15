#include "Aim.h"
#include "Player.h"
#include "PlayerCommand.h"
#include "../Engine/Camera.h"
#include "../Stage/CollisionMap.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/Enemymanager.h"
#include "../GameManager.h"
#include <vector>

//�f�o�b�O�p
#include "../Engine/Input.h"
#include "../Engine/Image.h"

namespace {
    static const float UP_MOUSE_LIMIT = -60.0f;
    static const float DOWN_MOUSE_LIMIT = 60.0f;
    
    static const int TARGET_CHANGE_COOLTIME = 30;
    static const int COMPULSION_TIME_DEFAULT = 60;                  //��������߂鎞��
    static const float HEIGHT_DISTANCE = 1.5f;                      //Aim�̍���
    static const float MOUSE_SPEED = 0.05f;                         //���x
    static const float HEIGHT_RAY = 0.1f;                           //RayCast�̒l�Ƀv���X���鍂��
    static const float MAX_CAMERA_OFFSET = 2.0f;                    //cameraOffset�̍ő勗��
    static const float SUPRESS = 0.002f;                            //Offset�̒l��}������
    static const float MOVE_SUPRESS = 0.03f;                        //�������̗}���̒l
    static const float STOP_SUPRESS = 0.06f;                        //�~�܂鎞�̗}���̒l
    static const float TARGET_RANGE = 25.0f;                        //�^�[�Q�b�g�̗L���͈�
    static const float FOV_RADIAN = XMConvertToRadians(120) / 2.0f;  //�^�[�Q�b�g�̗L���͈�
    static const float TARGET_RATIO = 0.2f;                         //�^�[�Q�b�g���̉�]��
}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPosition_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 }, cameraOffset_{ 0,0,0 },
    compulsionTarget_{ 0,0,0 }, compulsionPosisiton_{ 0,0,0 }, pPlayer_(nullptr), pEnemyBase_(nullptr), pCollisionMap_(nullptr),
    isMove_(true), isCompulsion_(false), isTarget_(false), compulsionTime_(0), iterations_(-1), sign_(1), range_(0), moveDistance_(0), 
    distanceDecrease_(0), center_{0,0,0,0}, shakeSpeed_(0), rangeDecrease_(0), isTargetChange_(false), targetChangeTime_(0), hPict_(-1)
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

    hPict_ = Image::Load("Image/TargetFound.png");
    assert(hPict_ >= 0);
    Transform foundTrans;
    foundTrans.position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    foundTrans.scale_ = XMFLOAT3(0.2f, 0.2f, 0.0f);
    Image::SetAlpha(hPict_, 255);
    Image::SetTransform(hPict_, foundTrans);

}

void Aim::Update()
{
    //�f�o�b�O�p
    if (Input::IsKeyDown(DIK_R)) isMove_ = !isMove_;
    if (Input::IsKey(DIK_X)) defPerspectDistance_ += 0.1f;
    if (Input::IsKey(DIK_Z)) defPerspectDistance_ -= 0.1f;
    if (Input::IsKeyDown(DIK_4)) SetCameraShake(3, 0.2f, 0.7f, 0.35f, 0.8f);

    if (compulsionTime_ > 0) {
        //�����ړ�
        if (isCompulsion_) {
            Compulsion();
            isCompulsion_ = false;
            return;
        }

        //�߂��Ă鎞�̈ړ�
        if (!isTarget_) {
            BackCompulsion();
            compulsionTime_--;
            return;
        }
    }

    if (isMove_) {
        //Target��Ԃ̈ړ��E��������߂��Ԃ�������Target��ԂɈړ�
        if (isTarget_) {
            CalcCameraOffset(0.0f);
            compulsionTime_ = 0;

            //�������AimTarget���̕`�悵�Ă݂�
            XMFLOAT3 tarPos = pEnemyBase_->GetPosition();
            tarPos.y += pEnemyBase_->GetAimTargetPos();
            XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&tarPos), Camera::GetViewMatrix());
            v2 = XMVector3TransformCoord(v2, Camera::GetProjectionMatrix());
            float x = XMVectorGetX(v2);
            float y = XMVectorGetY(v2);
            Transform foundTrans;
            foundTrans.position_ = XMFLOAT3(x, y, 0.0f);
            foundTrans.scale_ = XMFLOAT3(0.2f, 0.2f, 0.0f);
            Image::SetAlpha(hPict_, 255);
            Image::SetTransform(hPict_, foundTrans);
            
            //TargetChange
            XMFLOAT3 mouse = Input::GetMouseMove();  
            if (abs(mouse.x) > 30.0f) {
                if(targetChangeTime_ <= 0 && !isTargetChange_) ChangeTarget(mouse);
            }
            else {
                isTargetChange_ = false;
            }
            targetChangeTime_--;

            FacingTarget();
        }
        //�}�E�X�Ŏ��_�ړ�
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

}

void Aim::Release()
{
}

void Aim::TargetIsDead(EnemyBase* target)
{
    //����ł��獡�����Ă�������Ƀ^�[�Q�b�g�ł���Enemy������Ȃ炻�����^�[�Q�b�g�ɂ���
    if (pEnemyBase_ == target) {
        isTarget_ = false;
        SetTargetEnemy();
    }
}

void Aim::SetTargetEnemy()
{
    //���łɃ^�[�Q�b�g��Ԃ̏ꍇ�̓^�[�Q�b�g��������return
    if (isTarget_) {
        isTarget_ = false;
        pEnemyBase_ = nullptr;
        return;
    }

    EnemyManager* pEnemyManager = GameManager::GetEnemyManager();
    if (!pEnemyManager) return;
    std::vector<EnemyBase*> eList = pEnemyManager->GetAllEnemy();

    // �v���C���[�̎����������v�Z
    XMFLOAT3 playerForward;
    playerForward.x = (float)sin(XMConvertToRadians(transform_.rotate_.y));
    playerForward.y = 0.0f;
    playerForward.z = (float)cos(XMConvertToRadians(transform_.rotate_.y));

    float minLeng = 999999;
    int minLengIndex = -1;

    for (int i = 0; i < eList.size(); i++) {
        XMFLOAT3 ePos = eList.at(i)->GetPosition();

        //�^�[�Q�b�g�ւ̃x�N�g�����v�Z�i�t�x�N�g���j
        XMFLOAT3 toTarget = XMFLOAT3(cameraTarget_.x - ePos.x, 0.0f, cameraTarget_.z - ePos.z);

        //�^�[�Q�b�g�ւ̋������v�Z
        float distance = (float)sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z);

        //�͈͊O�������玟
        if (distance >= TARGET_RANGE) continue;

        // �����x�N�g���𐳋K��
        XMVECTOR toTargetNorm = XMVector3Normalize(XMLoadFloat3(&toTarget));
        XMVECTOR playerForwardNorm = XMLoadFloat3(&playerForward);

        // ����p���v�Z
        float dotProduct = XMVectorGetX(XMVector3Dot(toTargetNorm, playerForwardNorm));
        float angle = acosf(dotProduct);

        // �p�x���r���ă^�[�Q�b�g���͈͓��ɂ��邩�ǂ������m�F
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

void Aim::SetCameraShake(int iterat, float range, float range_decrease, float speed, float speed_decrease)
{
    iterations_ = iterat;
    sign_ = 1.0f;
    range_ = range;
    moveDistance_ = range_;
    distanceDecrease_ = speed_decrease;
    center_ = XMVectorZero();
    shakeSpeed_ = speed;
    rangeDecrease_ = range_decrease;
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

    //�v���C���[�̈ʒu���J�����̏œ_�Ƃ���        
    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    cameraTarget_ = { plaPos.x + cameraOffset_.x, plaPos.y + HEIGHT_DISTANCE, plaPos.z + cameraOffset_.z };

    CameraShake();

    //RayCast�̑O�ɏ�������
    perspectiveDistance_ = perspectiveDistance_ + ((defPerspectDistance_ - perspectiveDistance_) * 0.1f);
    XMVECTOR camPos = XMLoadFloat3(&cameraTarget_) + (direction * perspectiveDistance_);
    XMStoreFloat3(&cameraPosition_, camPos);

    //RayCast���Ă��̒l���㏑������
    RayCastStage();

    //�J���������Z�b�g
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

    //�����ړ�����Rotate�����߂�
    dir = XMVector3Normalize(dir);
    float rotationY = atan2f(XMVectorGetX(dir), XMVectorGetZ(dir));
    float rotationX = -asinf(XMVectorGetY(dir));
    transform_.rotate_.x = compulsionPosisiton_.x = XMConvertToDegrees(rotationX);
    transform_.rotate_.y = compulsionPosisiton_.y = XMConvertToDegrees(rotationY);

    //Rotate�̌��ʂ��g����AimDirection���v�Z���ăZ�b�g
    XMVECTOR direction = CalcDirection(transform_.rotate_.x, transform_.rotate_.y);
    XMStoreFloat3(&aimDirection_, -direction);
}

void Aim::BackCompulsion()
{
    CalcCameraOffset(0.0f);
    CalcMouseMove();

    //�������̎��_�̋��������߂�
    XMVECTOR dir = XMLoadFloat3(&cameraPosition_) - XMLoadFloat3(&cameraTarget_);
    float dist = XMVectorGetX(XMVector3Length(dir));
    perspectiveDistance_ = perspectiveDistance_ - ((dist - defPerspectDistance_) / (float)compulsionTime_);

    //�߂蒆��Position��Target���v�Z����
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

    //�v�Z���ʂ��g����RayCast���邻����Z�b�g
    RayCastStage();
    Camera::SetPosition(cameraPosition_);
    Camera::SetTarget(cameraTarget_);

    //�����ړ�����Rotate�EDirection�����߂�
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
    //�v���C���[�̕����Ɍ����悤�ɂ���
    XMVECTOR vFront{ 0,0,1,0 };
    float aimTarPos = pEnemyBase_->GetAimTargetPos();
    XMFLOAT3 targetPos = pEnemyBase_->GetPosition();
    targetPos = XMFLOAT3(targetPos.x, targetPos.y + aimTarPos, targetPos.z);

    XMFLOAT3 fAimPos = XMFLOAT3(cameraPosition_.x - targetPos.x, 0.0f, cameraPosition_.z - targetPos.z);
    XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);  //���K���p�̕ϐ���float�̒l������

    //�͈͊O�ɍs�����ꍇ�̏��������
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

    //�O�ϋ��߂Ĕ���]�������� angle �� -1 �|����
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos); //�O�ϋ��߂��� �O�ς̓x�N�g���^
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

    //-----------------------------X���v�Z-----------------------------
    // �J��������^�[�Q�b�g�ւ̕����x�N�g�����v�Z
    XMFLOAT3 taTarget = XMFLOAT3(targetPos.x, 0.0f, targetPos.z);
    XMFLOAT3 taCamPos = XMFLOAT3(cameraPosition_.x, 0.0f, cameraPosition_.z);
    XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&taTarget), XMLoadFloat3(&taCamPos));

    float distance = XMVectorGetX(XMVector3Length(direction));
    float height = cameraPosition_.y - targetPos.y;

    // �����ƍ����Ɋ�Â���X����]�p�x���v�Z
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

    // �v���C���[�̎����������v�Z
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

        //�͈͊O�������玟
        float distance = sqrtf(vec.x * vec.x + vec.z * vec.z);
        if (distance >= TARGET_RANGE) continue;

        //����p���v�Z
        float dotProduct = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vVec), vPlayerForward));
        float angle = acosf(dotProduct);
        if (angle > FOV_RADIAN) continue;

        //������ɍ��邩��E�E�������v�Z
        float dot = XMVectorGetY(XMVector3Cross(vPlayerForward, vVec));
        float fovRa = FOV_RADIAN;
        //�E�ɂ���
        if (dot > 0.0f && dot < minR) {
            minR = dot;
            rIndex = i;
        }
        //���ɂ���i������̏ꍇ��
        else if (dot < 0.0f && dot > minL) {
            minL = dot;
            lIndex = i;
        }
    }

    //�E��I��
    if (mouse.x > 0.0f) {
        if (rIndex != -1) {
            pEnemyBase_ = eList.at(rIndex);
            isTargetChange_ = true;
            targetChangeTime_ = TARGET_CHANGE_COOLTIME;
        }
    }
    //����I��
    else if (lIndex != -1) {
        pEnemyBase_ = eList.at(lIndex);
        isTargetChange_ = true;
        targetChangeTime_ = TARGET_CHANGE_COOLTIME;
    }
}

void Aim::CalcCameraOffset(float _aimMove)
{
    //�}�E�X�̈ړ��ʂ� offset�̒l��}��
    cameraOffset_.x += (cameraOffset_.x * -1.0f) * abs(_aimMove);
    cameraOffset_.z += (cameraOffset_.z * -1.0f) * abs(_aimMove);

    XMVECTOR vCameraOffset = XMLoadFloat3(&cameraOffset_);
    XMVECTOR vTargetOffset = pPlayer_->GetMovementVector();
    vTargetOffset *= MAX_CAMERA_OFFSET * -1;

    if (pPlayer_->GetCommand()->CmdWalk()) vCameraOffset += (vTargetOffset - vCameraOffset) * MOVE_SUPRESS;   //move
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

    //���C���������E���苗������������
    if (min <= (defPerspectDistance_)) {
        perspectiveDistance_ = min - HEIGHT_RAY;
    }

    XMVECTOR camPos = XMLoadFloat3(&cameraTarget_) + (vDir * perspectiveDistance_);
    XMStoreFloat3(&cameraPosition_, camPos);

}

void Aim::CalcMouseMove()
{
    XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾
    transform_.rotate_.y += (mouseMove.x * MOUSE_SPEED) * mouseSensitivity; //�������̉�]
    transform_.rotate_.x -= (mouseMove.y * MOUSE_SPEED) * mouseSensitivity; //�c�����̉�]
    if (transform_.rotate_.x <= UP_MOUSE_LIMIT) transform_.rotate_.x = UP_MOUSE_LIMIT;
    if (transform_.rotate_.x >= DOWN_MOUSE_LIMIT) transform_.rotate_.x = DOWN_MOUSE_LIMIT;

}

XMVECTOR Aim::CalcDirection(float x, float y)
{
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(y));
    XMMATRIX mView = mRotX * mRotY;
    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR direction = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    return XMVector3Normalize(direction);
}

void Aim::CameraShake()
{
    if (iterations_ < 0) return;

    //�J�����V�F�C�N�I���߂鏈��
    if (iterations_ == 0) {
        float dist = XMVectorGetX(XMVector3Length(center_));
        XMVECTOR vec = -XMVector3Normalize(center_);

        //�ڕW�n�_�ɂ���
        if (dist < moveDistance_ * shakeSpeed_) {
            moveDistance_ = dist;
            iterations_--;
            center_ = XMVectorZero();
        }

        vec *= moveDistance_ * shakeSpeed_;
        center_ += vec;

        cameraTarget_.x += center_.m128_f32[0];
        cameraTarget_.y += center_.m128_f32[1];
        cameraTarget_.z += center_.m128_f32[2];
        return;
    }

    //�ړ������E�ړ��ڕW�ꏊ���v�Z
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMVECTOR direction = XMVectorSet(sign_, 0.0f, 0.0f, 0.0f);
    XMVECTOR shakeDirection = XMVector3Normalize(XMVector3TransformNormal(direction, mRotY));
    XMVECTOR nextPosition = center_ + shakeDirection;

    //�������ׂĈړ�������
    XMVECTOR directionVector = nextPosition - center_;
    directionVector = XMVector3NormalizeEst(directionVector) * moveDistance_ * shakeSpeed_;
    center_ += directionVector;

    //�������������ׂ�
    bool sign2 = (center_.m128_f32[0] + center_.m128_f32[1] + center_.m128_f32[2] / 3) > 0.0f;
    bool sign3 = (directionVector.m128_f32[0] + directionVector.m128_f32[1] + directionVector.m128_f32[2] / 3) > 0;

    //�ڕW�n�_�𒴂��������ׂ�
    if (sign2 == sign3 && XMVectorGetX(XMVector3Length(center_)) > range_) {
        iterations_--;
        sign_ *= -1.0f;
        moveDistance_ *= distanceDecrease_;
        center_ = XMVector3Normalize(center_) * range_;
        range_ *= rangeDecrease_;
    }

    cameraTarget_.x += center_.m128_f32[0];
    cameraTarget_.y += center_.m128_f32[1];
    cameraTarget_.z += center_.m128_f32[2];
}
