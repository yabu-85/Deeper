#include "SearchAction.h"
#include "Character.h"
#include "CollisionMap.h"

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

    // �p�x���r���ă^�[�Q�b�g���͈͊O�ɂ��邩�ǂ������m�F�E�͈͊O�Ȃ�I��
    
    pCharacter_->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
    
    if (angle > fovRadian_) {
        
        pCharacter_->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

        return;
    }

    //�Ƃ肠����������CollisionMap�Ƃ̔������낤
    //�����x�N�g�����͈͓��ɓ�����Cell�� Segment/CollisionMap ���g���Ă����蔻������Ă���
    
    RayCastData data;
    data.start = charaPos;
    XMStoreFloat3(&data.dir, toTargetNorm);
    
    CollisionMap* pCollisionMap = (CollisionMap*)pCharacter_->FindObject("CollisionMap");
    pCollisionMap->RaySelectCellVsSegment(data, targetPos);

    //rayDist��toTarget���l���傫���Ȃ�Target��ڎ��o����
    float leng = XMVectorGetX(XMVector3Length(XMLoadFloat3(&toTarget)));
    isFoundTarget_ = false;
    if (data.hit && data.dist > leng) {
    //    isFoundTarget_ = true;
    //    OutputDebugString("on : Hit\n");
        pCharacter_->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
    }
    else {
    //    OutputDebugString("on : no  ");
    //    OutputDebugStringA(std::to_string(data.dist).c_str() );
    //    OutputDebugString("\n");
    }

}

//------------------------------Audio---------------------------------

AuditorySearchAction::AuditorySearchAction(Character* obj) : BaseAction(obj)
{
}

void AuditorySearchAction::Update()
{
}

