#include "SearchAction.h"
#include "Character.h"

#include "Engine/Input.h"
#include <math.h>

VisionSearchAction::VisionSearchAction(Character* obj, float range, float fov) : BaseAction(obj), range_(range), isFoundTarget_(false)
{
    pTarget_ = (Character*)pCharacter_->FindObject("Player");
    fovRadian_ = XMConvertToRadians(fov) / 2;
}

void VisionSearchAction::Update()
{
    // Character�̎����������v�Z
    XMFLOAT3 charaPos = pCharacter_->GetPosition();
    XMFLOAT3 targetPos = pTarget_->GetPosition();

    //�^�[�Q�b�g�ւ̃x�N�g�����v�Z�i�t�x�N�g���j
    XMFLOAT3 toTarget = XMFLOAT3(charaPos.x - targetPos.x, 0.0f, charaPos.z - targetPos.z);

    float charaRotateY = pCharacter_->GetRotate().y - 180.0f;
    XMFLOAT3 charaForward = XMFLOAT3();
    charaForward.x = (float)sin(XMConvertToRadians(charaRotateY));
    charaForward.z = (float)cos(XMConvertToRadians(charaRotateY));

    // �����x�N�g���𐳋K��
    XMVECTOR toTargetNorm = XMVector3Normalize(XMLoadFloat3(&toTarget));
    XMVECTOR charaForwardNorm = XMLoadFloat3(&charaForward);

    // ����p���v�Z
    float dotProduct = XMVectorGetX(XMVector3Dot(toTargetNorm, charaForwardNorm));
    float angle = acosf(dotProduct);

    // �p�x���r���ă^�[�Q�b�g���͈͓��ɂ��邩�ǂ������m�F
    isFoundTarget_ = false;
    if (angle <= fovRadian_) {
        //���������
        isFoundTarget_ = true;
    }


    //�Ƃ肠����������CollisionMap�Ƃ̔������낤
    //�����x�N�g�����͈͓��ɓ�����Cell�� Segment/CollisionMap ���g���Ă����蔻������Ă���


    // line �x�N�g���ɐ����ȃx�N�g�����v�Z
    XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // �������̃x�N�g�����K�v�Ȃ�ύX
    XMVECTOR lineNormal = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&charaForward), upVector));

    // �l�p�`�̎l���̓_�����߂�
    XMVECTOR c1 = XMVectorSet(20, 0, 20, 0);
    XMVECTOR c2 = XMVectorSet(20, 0, 30, 0);
    XMVECTOR c3 = XMVectorSet(30, 0, 30, 0);
    XMVECTOR c4 = XMVectorSet(30, 0, 20, 0);

    // �l���̓_������̃x�[�X�ƂȂ�_�̋��������߂�
    XMVECTOR lineBase = XMLoadFloat3(&charaPos);
    c1 = c1 - lineBase;
    c2 = c2 - lineBase;
    c3 = c3 - lineBase;
    c4 = c4 - lineBase;

    // ���ςɂ����̕��������߂�
    float dp1 = XMVectorGetY(XMVector3Dot(lineNormal, c1));
    float dp2 = XMVectorGetY(XMVector3Dot(lineNormal, c2));
    float dp3 = XMVectorGetY(XMVector3Dot(lineNormal, c3));
    float dp4 = XMVectorGetY(XMVector3Dot(lineNormal, c4));

    // �S�����������ɂ���΁A���Ǝl�p�`���������Ă��邱�Ƃ͂Ȃ�
    if ((dp1 * dp2 <= 0) || (dp2 * dp3 <= 0) || (dp3 * dp4 <= 0)) {
        OutputDebugString("Hit\n");
    }
    else {
        OutputDebugString("\n");
    }


    //Cell��Triangles�ɑ΂���

    


}

//------------------------------Audio---------------------------------

AuditorySearchAction::AuditorySearchAction(Character* obj) : BaseAction(obj)
{
}

void AuditorySearchAction::Update()
{
}

