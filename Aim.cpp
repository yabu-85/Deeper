#include "Aim.h"
#include "Engine/Camera.h"
#include "Player.h"
#include "CollisionMap.h"
#include <vector>
#include "EnemyBase.h"
#include "Enemymanager.h"
#include "GameManager.h"
#include "PlayerCommand.h"
#include "Engine/Input.h"
#include "Engine/Text.h"

namespace {
    static const float HEIGHT_DISTANCE = 3.0f;
    static const float UP_MOUSE_LIMIT = -60.0f;
    static const float DOWN_MOUSE_LIMIT = 60.0f;
    static const float MOUSE_SPEED = 0.05f;
    static const float HEIGHT_RAY = 0.1f;                           //RayCast�̒l�Ƀv���X���鍂��
    static const float MAX_CAMERA_OFFSET = 2.0f;                    //cameraOffset�̍ő勗��
    static const float SUPRESS = 0.002f;                            //Offset�̒l��}������
    static const float MOVE_SUPRESS = 0.03f;                        //�������̗}���̒l
    static const float STOP_SUPRESS = 0.06f;                        //�~�܂鎞�̗}���̒l
    static const float TARGET_RANGE = 50.0f;                        //�^�[�Q�b�g�̗L���͈�
    static const float FOV_RADIAN = XMConvertToRadians(60) / 2.0f;  //�^�[�Q�b�g�̗L���͈�
    static const float TARGET_RATIO = 0.2f;                         //�^�[�Q�b�g���̉�]��
    
    static const float M_PI = 3.141592653589793f;
    
    int waruNumDefault = 60;
    int waruNum = 0;
    Text* text = nullptr;
}

// �C�[�W���O�֐��Ƃ��Ă̐��`��ԁiLerp�j
float EaseInOutLerp(float t)
{
    // �C�[�W���O�֐��i��: Cubic EaseInOut�j
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) * 0.5f;
}

// Lerp�֐�
float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

// �l���قȂ�Ȃ炾�񂾂�߂鏈��
float GradualReturn(float defaultNum, float num, float t)
{
    // �C�[�W���O�֐���K�p���Đ��`���
    return Lerp(num, defaultNum, EaseInOutLerp(t));
}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 }, plaPos_{ 0,0,0 }, cameraOffset_{ 0,0,0 },
    compulsionTarget_{0,0,0}, compulsionPos_{0,0,0}, pPlayer_(nullptr), pEnemyBase_(nullptr), pCollisionMap_(nullptr), 
    isMove_(false), isCompulsion_(false), isTarget_(false)
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
    isMove_ = true;

    text = new Text();
    text->Initialize();

}

void Aim::Update()
{
    //�f�o�b�O�p
    if (Input::IsKeyDown(DIK_T)) isMove_ = !isMove_;
    if (Input::IsKey(DIK_X)) defPerspectDistance_ += 0.1f;
    if (Input::IsKey(DIK_Z)) defPerspectDistance_ -= 0.1f;

    //�����ړ�
    if (isCompulsion_) {
        XMStoreFloat3(&cameraPos_, (XMVectorLerp(XMLoadFloat3(&cameraPos_), XMLoadFloat3(&compulsionPos_), 0.05f)));
        XMStoreFloat3(&cameraTarget_, (XMVectorLerp(XMLoadFloat3(&cameraTarget_), XMLoadFloat3(&compulsionTarget_), 0.05f)));
        RayCastStage();

        Camera::SetPosition(cameraPos_);
        Camera::SetTarget(cameraTarget_);
        isCompulsion_ = false;

        XMVECTOR dir = XMLoadFloat3(&cameraPos_) - XMLoadFloat3(&cameraTarget_);
        perspectiveDistance_ = XMVectorGetX(XMVector3Length(dir));

        //�����ړ�����Rotate�����߂�
        dir = XMVector3Normalize(dir);
        float rotationY = atan2f(XMVectorGetX(dir), XMVectorGetZ(dir));
        float rotationX = -asinf(XMVectorGetY(dir));
        transform_.rotate_.x = XMConvertToDegrees(rotationX);
        transform_.rotate_.y = XMConvertToDegrees(rotationY);

        //Rotate�̌��ʂ��g����AimDirection���v�Z���ăZ�b�g
        XMMATRIX mRotX = XMMatrixRotationX(rotationX);
        XMMATRIX mRotY = XMMatrixRotationY(rotationY);
        XMMATRIX mView = mRotX * mRotY;
        const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        XMVECTOR direction = XMVector3TransformNormal(forwardVector, mView);
        XMVector3Normalize(direction);
        XMStoreFloat3(&aimDirection_, -direction);
        return;
    }
    
    //airMove_���I�t�̏�Ԃ�Aim�͂��Ă��邪�A�������邱�Ƃ͂Ȃ��Ȃ���
    if (isMove_) {

        //Target������ꍇ�̏���
        if (isTarget_) {
            CalcCameraOffset(0.0f);

            //�^�[�Q�b�g�������Ă�Ȃ炻����Aim���킹��
            if (!pEnemyBase_->IsDead()) FacingTarget();
            else {
                //����ł��獡�����Ă�������Ƀ^�[�Q�b�g�ł���Enemy������Ȃ炻�����^�[�Q�b�g�ɂ���
                isTarget_ = false;
                SetTargetEnemy();
            }
        }

        //Target�����Ȃ��ꍇ�̏����F�}�E�X�Ŏ��_�ړ�
        else {
            XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾
            transform_.rotate_.y += (mouseMove.x * MOUSE_SPEED) * mouseSensitivity; //�������̉�]
            transform_.rotate_.x -= (mouseMove.y * MOUSE_SPEED) * mouseSensitivity; //�c�����̉�]
            if (transform_.rotate_.x <= UP_MOUSE_LIMIT) transform_.rotate_.x = UP_MOUSE_LIMIT;
            if (transform_.rotate_.x >= DOWN_MOUSE_LIMIT) transform_.rotate_.x = DOWN_MOUSE_LIMIT;
            CalcCameraOffset(mouseMove.x * SUPRESS);
        }
    }
    else {
        CalcCameraOffset(0.0f);
    }

    XMVECTOR dir = XMLoadFloat3(&compulsionPos_) - XMLoadFloat3(&compulsionTarget_);
    dir = XMVector3Normalize(dir);
    float rotationY = atan2f(XMVectorGetX(dir), XMVectorGetZ(dir));
    float rotationX = -asinf(XMVectorGetY(dir));
    rotationY = XMConvertToDegrees(rotationY);
    rotationX = XMConvertToDegrees(rotationX);

    OutputDebugString("X : ");
    OutputDebugStringA(std::to_string(rotationX).c_str());
    OutputDebugString(" , Y : ");
    OutputDebugStringA(std::to_string(rotationY).c_str());
    OutputDebugString("\n");

    OutputDebugString("X : ");
    OutputDebugStringA(std::to_string(transform_.rotate_.x).c_str());
    OutputDebugString(" , Y : ");
    OutputDebugStringA(std::to_string(transform_.rotate_.y).c_str());
    OutputDebugString("\n");
    OutputDebugString("\n");

    //�ł����ق��ɏ������̈���
    //transform_.rotate_.x > rotationX ? transform_.rotate_.x = transform_.rotate_.x - rotationX : transform_.rotate_.x = rotationX - transform_.rotate_.x;
    //transform_.rotate_.y > rotationY ? transform_.rotate_.y = transform_.rotate_.y - rotationY : rotationY = transform_.rotate_.y - transform_.rotate_.y;

    //�J�����̉�]
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX mView = mRotX * mRotY;

    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR direction = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    XMVector3Normalize(direction);
    XMStoreFloat3(&aimDirection_, -direction);

    if (waruNum > 0) {
        //�v���C���[�̈ʒu���J�����̏œ_�Ƃ���
        plaPos_ = pPlayer_->GetPosition();
        cameraTarget_.x = plaPos_.x + cameraOffset_.x / waruNum;
        cameraTarget_.y = plaPos_.y + HEIGHT_DISTANCE;
        cameraTarget_.z = plaPos_.z + cameraOffset_.z / waruNum;
    }
    else {
        //�v���C���[�̈ʒu���J�����̏œ_�Ƃ���
        plaPos_ = pPlayer_->GetPosition();
        cameraTarget_.x = plaPos_.x + cameraOffset_.x;
        cameraTarget_.y = plaPos_.y + HEIGHT_DISTANCE;
        cameraTarget_.z = plaPos_.z + cameraOffset_.z;
    }

    perspectiveDistance_ = GradualReturn(defPerspectDistance_, perspectiveDistance_, 0.1f);

    //RayCast�̑O�ɏ�������
    XMVECTOR camPos = XMLoadFloat3(&cameraTarget_) + (direction * perspectiveDistance_);
    XMStoreFloat3(&cameraPos_, camPos);

    //�����ړ�����߂��Ă鎞�̏���
    if (waruNum > 0) {
        //�ړ������̌v�Z
        XMVECTOR pVec = XMLoadFloat3(&cameraPos_) - XMLoadFloat3(&compulsionPos_);

        //�ړ�����
        XMVECTOR tv = pVec / (float)waruNum;
        XMStoreFloat3(&cameraPos_, XMLoadFloat3(&compulsionPos_) + tv);
        waruNum--;
    }

    //RayCast���Ă��̒l���㏑������
    RayCastStage();

    //�ŏI���ʂ������ړ����ɂ����
    compulsionPos_ = cameraPos_;
    compulsionTarget_ = cameraTarget_;

    //�J���������Z�b�g
    Camera::SetPosition(cameraPos_);
    Camera::SetTarget(cameraTarget_);
}

void Aim::Draw()
{
    text->Draw(30, 300, cameraPos_.x);
    text->Draw(30, 340, cameraPos_.y);
    text->Draw(30, 380, cameraPos_.z);

    text->Draw(30, 450, cameraTarget_.x);
    text->Draw(30, 490, cameraTarget_.y);
    text->Draw(30, 530, cameraTarget_.z);

}

void Aim::Release()
{
}

void Aim::SetTargetEnemy()
{
    //���łɃ^�[�Q�b�g��Ԃ̏ꍇ�̓^�[�Q�b�g��������return
    if (isTarget_) {
        isTarget_ = false;
        return;
    }

    EnemyManager* pEnemyManager = GameManager::GetEnemyManager();
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

void Aim::SetCompulsion(XMFLOAT3 pos, XMFLOAT3 tar)
{
    compulsionPos_ = pos;
    compulsionTarget_ = tar;
    isCompulsion_ = true;
    waruNum = waruNumDefault;

}

//-----------private--------------------------------------------

void Aim::FacingTarget()
{
    //�v���C���[�̕����Ɍ����悤�ɂ���
    XMVECTOR vFront{ 0,0,1,0 };
    float aimTarPos = pEnemyBase_->GetAimTargetPos();
    XMFLOAT3 targetPos = pEnemyBase_->GetPosition();
    targetPos = XMFLOAT3(targetPos.x, targetPos.y + aimTarPos, targetPos.z);

    XMFLOAT3 fAimPos = XMFLOAT3(cameraPos_.x - targetPos.x, 0.0f, cameraPos_.z - targetPos.z);
    XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);  //���K���p�̕ϐ���float�̒l������
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
    XMFLOAT3 taCamPos = XMFLOAT3(cameraPos_.x, 0.0f, cameraPos_.z);
    XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&taTarget), XMLoadFloat3(&taCamPos));

    float distance = XMVectorGetX(XMVector3Length(direction));
    float height = cameraPos_.y - targetPos.y;

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

void Aim::CalcCameraOffset(float _aimMove)
{
    //�}�E�X�̈ړ��ʂ� offset�̒l��}��
    cameraOffset_.x += (cameraOffset_.x * -1.0f) * abs(_aimMove);
    cameraOffset_.z += (cameraOffset_.z * -1.0f) * abs(_aimMove);
    
    XMVECTOR vCameraOffset = XMLoadFloat3(&cameraOffset_);
    XMVECTOR vTargetOffset = pPlayer_->GetMovement();
    vTargetOffset *= MAX_CAMERA_OFFSET * -1;

    if(pPlayer_->GetCommand()->CmdWalk()) vCameraOffset += (vTargetOffset - vCameraOffset) * MOVE_SUPRESS;   //move
    else vCameraOffset += (vTargetOffset - vCameraOffset) * STOP_SUPRESS;                     //stop

    XMStoreFloat3(&cameraOffset_, vCameraOffset);
}

void Aim::RayCastStage()
{
    pCollisionMap_ = static_cast<CollisionMap*>(FindObject("CollisionMap"));
    if (pCollisionMap_ == nullptr) return;

    RayCastData data;
    XMFLOAT3 start = cameraTarget_;
    XMVECTOR vDir = XMLoadFloat3(&cameraPos_) - XMLoadFloat3(&cameraTarget_);
    vDir = XMVector3Normalize(vDir);
    XMFLOAT3 dir = XMFLOAT3();
    XMStoreFloat3(&dir, vDir);
    data.start = start;
    data.dir = dir;
    float min = 0.0f;
    pCollisionMap_->GetRayCastMinDist(cameraPos_, pPlayer_->GetPosition(), &data, min);

    //���C���������E���苗������������
    if (min <= (defPerspectDistance_)) {
        perspectiveDistance_ = min - HEIGHT_RAY;
    }
    else {
        perspectiveDistance_ = GradualReturn(defPerspectDistance_, perspectiveDistance_, 0.1f);

        perspectiveDistance_ = defPerspectDistance_ - HEIGHT_RAY;
    }

    XMVECTOR camPos = XMLoadFloat3(&cameraTarget_) + (vDir * perspectiveDistance_);
    XMStoreFloat3(&cameraPos_, camPos);

}