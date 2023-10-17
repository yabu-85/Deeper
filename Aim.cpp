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
#include "GameManager.h"

namespace {
    const float upMouselimit = -89.0f;
    const float donwMouselimit = 70.0f;
    const float mouseSpeed = 0.05f;

    const float defPerspectDistance = 8.0f; //�f�t�H���g�̎��_�̋���
    const float heightRay = 0.5f;           //Stage�Ƃ�Ray����̓������������Ƀv���X����l

    const float numSupress = 0.002f;        //�}�E�X�ړ���Offset�̒l��߂���
    const float maxCameraOffset = 2.0f;     //cameraOffset�̍ő勗��
    const float moveAimTime = 0.05f;        //�������̗}���̒l
    const float stopAimTime = 0.05f;        //�~�܂鎞�̗}���̒l

    const float targetRange = 30.0f;        //�^�[�Q�b�g�̗L���͈�
    const float fovRadian = XMConvertToRadians(60) / 2;
}

float easeOutQuad(float number) {
    return 1.0f - (1.0f - number) * (1.0f - number);
}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 },
    plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimMove_(false), cameraOffset_{0,0,0}, isTarget_(false), pEnemyBase_(nullptr)
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

    if (isTarget_) {
        //�J�����I�t�Z�b�g�̋@�\�̓^�[�Q�b�g�����L���ɂ���A�������}�E�X�ł̗}���͂Ȃ�
        CalcCameraOffset(0.0f);


        //test
        if (Input::IsKey(DIK_K)) pEnemyBase_->KillMe();


        //isTarget��ԂȂ̂�Target�ƂȂ�Enemy�����Ȃ��ꍇreset
        if (!pEnemyBase_->IsDead()) FacingTarget();
        else isTarget_ = false;
        
    }
    else {
        XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾
        transform_.rotate_.y += (mouseMove.x * mouseSpeed) * mouseSensitivity; //�������̉�]
        transform_.rotate_.x -= (mouseMove.y * mouseSpeed) * mouseSensitivity; //�c�����̉�]
        if (transform_.rotate_.x <= upMouselimit) transform_.rotate_.x = upMouselimit;
        if (transform_.rotate_.x >= donwMouselimit) transform_.rotate_.x = donwMouselimit;

        CalcCameraOffset(mouseMove.x * numSupress);
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

    RayCastStage(cameraTarget_);

    //�J�����œ_
    XMVECTOR caTarget = XMLoadFloat3(&cameraTarget_);

    //����Aim�̋߂��̒l���v���C���[����擾���Čv�Z�����Ă�
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
    if (isTarget_) {
        isTarget_ = false;
        return;
    }

    GameManager* pGameManager = (GameManager*)FindObject("GameManager");
    std::vector<EnemyBase*> eList = pGameManager->GetAllEnemy();

    // �v���C���[�̎����������v�Z
    XMFLOAT3 playerForward;
    playerForward.x = sin(XMConvertToRadians(transform_.rotate_.y));
    playerForward.y = 0.0f;
    playerForward.z = cos(XMConvertToRadians(transform_.rotate_.y));

    float minLeng = 999999;
    int minLengIndex = -1;
    int index = 0;

    for (EnemyBase* e : eList) {
        index++;
        XMFLOAT3 ePos = e->GetPosition();

        // �v���C���[����^�[�Q�b�g�ւ̃x�N�g�����v�Z�i�t�x�N�g���j
        XMFLOAT3 toTarget;
        toTarget.x = cameraTarget_.x - ePos.x;
        toTarget.y = 0.0f;
        toTarget.z = cameraTarget_.z - ePos.z;

        // �v���C���[����^�[�Q�b�g�ւ̋������v�Z
        float distance = sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z);

        //�͈͊O�������玟
        if (distance >= targetRange) continue;

        // �v���C���[�ƃ^�[�Q�b�g�̕����x�N�g���𐳋K��
        XMVECTOR toTargetNorm = XMVector3Normalize(XMLoadFloat3(&toTarget));
        XMVECTOR playerForwardNorm = XMLoadFloat3(&playerForward);

        // �v���C���[�̎���p���v�Z
        float dotProduct = XMVectorGetX(XMVector3Dot(toTargetNorm, playerForwardNorm));
        float angle = acosf(dotProduct);

        // �����Ɗp�x���r���ă^�[�Q�b�g���͈͓��ɂ��邩�ǂ������m�F
        if (angle <= fovRadian) {
            if (minLeng > distance) {
                minLeng = distance;
                minLengIndex = index - 1;
            }
        }
    }

    if (minLengIndex >= 0) {
        pEnemyBase_ = eList.at(minLengIndex);
        isTarget_ = true;
    }

}

//-----------private-------------------

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
    float angle = acos(dot);

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
    float rotationX = atan2(height, distance);

    // ��]�p�x��x���ɕϊ�
    rotationX = -XMConvertToDegrees(rotationX);

    transform_.rotate_.x = rotationX;
    if (transform_.rotate_.x <= upMouselimit) transform_.rotate_.x = upMouselimit;
    if (transform_.rotate_.x >= donwMouselimit) transform_.rotate_.x = donwMouselimit;

}

void Aim::CalcCameraOffset(float _aimMove)
{
    //�}�E�X�̈ړ��ʂ� offset�̒l��}��
    cameraOffset_.x += (cameraOffset_.x * -1.0f) * abs(_aimMove);
    cameraOffset_.z += (cameraOffset_.z * -1.0f) * abs(_aimMove);
    
    XMVECTOR vCameraOffset = XMLoadFloat3(&cameraOffset_);
    XMVECTOR vTargetOffset;

    //�ړ��L�[�̏����擾���������t��
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

        //���C���������E���苗������������
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