#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Stage.h"
#include <vector>

namespace {
    const float defaultPerspectiveDistance_ = 7.0f;
    const float heightRay = 0.5f; //Stage�Ƃ�Ray����̓������������Ƀv���X����l

}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 },
    plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimMove_(false)
{
    mouseSensitivity = 2.0f;
    perspectiveDistance_ = 7.0f;
    heightDistance_ = 1.5f;
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
    if (!aimMove_) return;

    XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾

    //�ړ��ʂ��v�Z
    transform_.rotate_.y += (mouseMove.x * 0.05f) * mouseSensitivity; //�������̉�]
    transform_.rotate_.x -= (mouseMove.y * 0.05f) * mouseSensitivity; //�c�����̉�]
    if (transform_.rotate_.x <= -89.0f) transform_.rotate_.x = -89.0f;
    if (transform_.rotate_.x >= 89.0f) transform_.rotate_.x = 89.0f;

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

    //�}�C�i�X�ō��A�v���X�ŉE���ɂȂ�
    //����̒l�͑��������ǁA��ʏ�̒��S���牽����܂ł��Ċ������Ǝv���F�v���C���[�Ƃ̋������߂��ƒl���������Ȃ�A�����Ƒ傫��
    float cameraOffset = 0.0f;

    cameraTarget_.x = plaPos_.x + (aimDirection_.z * cameraOffset);
    cameraTarget_.y = plaPos_.y + heightDistance_;
    cameraTarget_.z = plaPos_.z - (aimDirection_.x * cameraOffset);

    //�J�����œ_
    XMVECTOR caTarget = XMLoadFloat3(&cameraTarget_);

    RayCastStage();

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

void Aim::RayCastStage()
{
    Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g��T��
    std::vector<IntersectData> datas = pStage->GetModelHandle();    //���f���ԍ����擾

    RayCastData data;
    XMFLOAT3 start = pPlayer_->GetPosition();
    start.y += heightDistance_;
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

        //���C���������E���苗������������
        if (data.hit && data.dist < (defaultPerspectiveDistance_ + heightRay))
        {
            float dist = data.dist;
            dist -= heightRay;
            perspectiveDistance_ = dist;
            rayHit = true;
        }
    }

    if (!rayHit) {
        perspectiveDistance_ = defaultPerspectiveDistance_;
    }

    return;
}
