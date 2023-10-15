#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"
#include "Triangle.h"
#include <vector>

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
    if (Input::IsMouseButtonDown(0)) aimMove_ = !aimMove_;

    if (!aimMove_) return;

    XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾

    //�ړ��ʂ��v�Z
    transform_.rotate_.y += (mouseMove.x * mouseSpeed) * mouseSensitivity; //�������̉�]
    transform_.rotate_.x -= (mouseMove.y * mouseSpeed) * mouseSensitivity; //�c�����̉�]
    if (transform_.rotate_.x <= upMouselimit) transform_.rotate_.x = upMouselimit;
    if (transform_.rotate_.x >= donwMouselimit) transform_.rotate_.x = donwMouselimit;

    //�J�����̉�]
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    //�J�����̈ʒu�Ɖ�]������
    XMMATRIX mView = mRotX * mRotY;

    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR camPos = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    XMVector3Normalize(camPos);
    XMStoreFloat3(&aimDirection_, -camPos);

    //�v���C���[�̈ʒu���J�����̏œ_�Ƃ���
    plaPos_ = pPlayer_->GetPosition();

    CalcCameraOffset(mouseMove.x * numSupress);
    cameraTarget_.x = plaPos_.x + cameraOffset_.x;
    cameraTarget_.y = plaPos_.y + heightDistance_;
    cameraTarget_.z = plaPos_.z + cameraOffset_.z;

    RayCastStage(cameraTarget_);

    //�J�����œ_
    XMVECTOR caTarget = XMLoadFloat3(&cameraTarget_);

    //�v���C���[�̔��a���l�����ĉ�]��K�p���Ă���
    //����Aim�̋߂��̒l���v���C���[����擾���Čv�Z�����Ă�
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