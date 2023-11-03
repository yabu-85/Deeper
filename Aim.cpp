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

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 },
    plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimMove_(false), cameraOffset_{0,0,0}, isTarget_(false), pEnemyBase_(nullptr), pCollisionMap_(nullptr)
{
    mouseSensitivity = 2.0f;
    heightDistance_ = 3.0f;
    upMouselimit_ = -89.0f;
    donwMouselimit_ = 70.0f;
    mouseSpeed_ = 0.05f;
    defPerspectDistance_ = 10.0f;
    perspectiveDistance_ = defPerspectDistance_;
    heightRay_ = 0.1f;
    numSupress_ = 0.002f;
    maxCameraOffset_ = 2.0f;
    moveAimTime_ = 0.04f;
    stopAimTime_ = 0.035f;
    targetRange_ = 50.0f;
    fovRadian_ = XMConvertToRadians(60) / 2;
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
    //�f�o�b�O�p
    if (Input::IsKeyDown(DIK_T)) aimMove_ = !aimMove_;
    if (Input::IsKey(DIK_X)) defPerspectDistance_ += 0.1f;
    if (Input::IsKey(DIK_Z)) defPerspectDistance_ -= 0.1f;
    
    if (!aimMove_) return;

    if (isTarget_) {
        CalcCameraOffset(0.0f);

        //isTarget��ԂȂ̂�Target�ƂȂ�Enemy�����Ȃ��ꍇreset
        if (!pEnemyBase_->IsDead()) FacingTarget();
        else isTarget_ = false;
    }
    else {
        XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾
        transform_.rotate_.y += (mouseMove.x * mouseSpeed_) * mouseSensitivity; //�������̉�]
        transform_.rotate_.x -= (mouseMove.y * mouseSpeed_) * mouseSensitivity; //�c�����̉�]
        if (transform_.rotate_.x <= upMouselimit_) transform_.rotate_.x = upMouselimit_;
        if (transform_.rotate_.x >= donwMouselimit_) transform_.rotate_.x = donwMouselimit_;

        CalcCameraOffset(mouseMove.x * numSupress_);
    }

    //�J�����̉�]
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    
    //�J�����̈ʒu�Ɖ�]������
    XMMATRIX mView = mRotX * mRotY;

    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR direction = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    XMVector3Normalize(direction);
    XMStoreFloat3(&aimDirection_, -direction);

    //�v���C���[�̈ʒu���J�����̏œ_�Ƃ���
    plaPos_ = pPlayer_->GetPosition();
    cameraTarget_.x = plaPos_.x + cameraOffset_.x;
    cameraTarget_.y = plaPos_.y + heightDistance_;
    cameraTarget_.z = plaPos_.z + cameraOffset_.z;

    //RayCast�O�̏�������
    XMVECTOR caTarget = XMLoadFloat3(&cameraTarget_);
    XMVECTOR camPos = caTarget + (direction * perspectiveDistance_);
    XMStoreFloat3(&cameraPos_, camPos);
    XMStoreFloat3(&cameraTarget_, caTarget);

    //RayCast���Ă��̒l���㏑������
    RayCastStage(cameraTarget_);
    camPos = caTarget + (direction * perspectiveDistance_);
    XMStoreFloat3(&cameraPos_, camPos);

    Camera::SetPosition(cameraPos_);
    Camera::SetTarget(cameraTarget_);
}

void Aim::Draw()
{
}

void Aim::Release()
{
}

XMFLOAT3 Aim::GetTargetPos()
{
    if(!isTarget_)
        return XMFLOAT3();

    return pEnemyBase_->GetPosition();
}

void Aim::SetTargetEnemy()
{
    //�^�[�Q�b�g�����̏ꍇ��return
    if (isTarget_) {
        isTarget_ = false;
        return;
    }

    GameManager* pGM = (GameManager*)FindObject("GameManager");
    EnemySpawnCtrl* pEnemySpawnCtrl = pGM->GetEnemySpawnCtrl();
    std::vector<EnemyBase*> eList = pEnemySpawnCtrl->GetAllEnemy();

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
        if (distance >= targetRange_) continue;

        // �����x�N�g���𐳋K��
        XMVECTOR toTargetNorm = XMVector3Normalize(XMLoadFloat3(&toTarget));
        XMVECTOR playerForwardNorm = XMLoadFloat3(&playerForward);

        // ����p���v�Z
        float dotProduct = XMVectorGetX(XMVector3Dot(toTargetNorm, playerForwardNorm));
        float angle = acosf(dotProduct);

        // �p�x���r���ă^�[�Q�b�g���͈͓��ɂ��邩�ǂ������m�F
        if (angle <= fovRadian_) {
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
    //�v���C���[�̕����Ɍ����悤�ɂ���
    XMVECTOR vFront{ 0,0,1,0 };
    XMFLOAT3 targetPos = pEnemyBase_->GetPosition();

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
    transform_.rotate_.y = XMConvertToDegrees(angle);

    //-----------------------------X���v�Z-----------------------------
    // �J��������^�[�Q�b�g�ւ̕����x�N�g�����v�Z
    XMFLOAT3 taTarget = XMFLOAT3(targetPos.x, 0.0f, targetPos.z);
    XMFLOAT3 taCamPos = XMFLOAT3(cameraPos_.x, 0.0f, cameraPos_.z);
    XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&taTarget), XMLoadFloat3(&taCamPos));

    // �J�����ƃ^�[�Q�b�g�Ԃ̋������v�Z
    float distance = XMVectorGetX(XMVector3Length(direction));

    // �J�����ƃ^�[�Q�b�g�Ԃ̍��������v�Z
    float height = cameraPos_.y - targetPos.y;

    // �����ƍ����Ɋ�Â���X����]�p�x���v�Z
    float rotationX = (float)atan2(height, distance);

    // ��]�p�x��x���ɕϊ�
    rotationX = -XMConvertToDegrees(rotationX);

    transform_.rotate_.x = rotationX;
    if (transform_.rotate_.x <= upMouselimit_) transform_.rotate_.x = upMouselimit_;
    if (transform_.rotate_.x >= donwMouselimit_) transform_.rotate_.x = donwMouselimit_;

}

void Aim::CalcCameraOffset(float _aimMove)
{
    //�}�E�X�̈ړ��ʂ� offset�̒l��}��
    cameraOffset_.x += (cameraOffset_.x * -1.0f) * abs(_aimMove);
    cameraOffset_.z += (cameraOffset_.z * -1.0f) * abs(_aimMove);
    
    XMVECTOR vCameraOffset = XMLoadFloat3(&cameraOffset_);
    XMVECTOR vTargetOffset = pPlayer_->GetMoveVec();
    vTargetOffset *= maxCameraOffset_ * -1;

    if(pPlayer_->IsMove()) vCameraOffset += (vTargetOffset - vCameraOffset) * moveAimTime_;   //move
    else vCameraOffset += (vTargetOffset - vCameraOffset) * stopAimTime_;                     //stop

    XMStoreFloat3(&cameraOffset_, vCameraOffset);
}

void Aim::RayCastStage(XMFLOAT3 _start)
{
    pCollisionMap_ = (CollisionMap*)FindObject("CollisionMap");
    if (pCollisionMap_ == nullptr) return;

    RayCastData data;
    XMFLOAT3 start = _start;
    XMFLOAT3 dir = XMFLOAT3(cameraPos_.x - start.x, cameraPos_.y - start.y, cameraPos_.z - start.z);
    XMVECTOR vDir = XMLoadFloat3(&dir);
    vDir = XMVector3Normalize(vDir);
    XMStoreFloat3(&dir, vDir);
    data.start = start;
    data.dir = dir;
    float min = pCollisionMap_->GetRayCastMinDist(cameraPos_, &data);

    //���C���������E���苗������������
    if (min <= (defPerspectDistance_)) {
        perspectiveDistance_ = min - heightRay_;
    }
    else {
        perspectiveDistance_ = defPerspectDistance_ - heightRay_;
    }

}